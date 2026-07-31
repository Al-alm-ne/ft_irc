/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:48:03 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/31 14:04:14 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ModeCommand.hpp"
#include "../include/Replies.hpp"
#include <cstdlib>

ModeCommand::ModeCommand(Server &server) : _server(server) {}
ModeCommand::~ModeCommand() {}

/**
 * @brief High-level executor for the MODE command.
 * Validates syntax, channel existence, and routes the request to either
 * a "view modes" response or an active mode mutation process.
 * @param client The client issuing the command.
 * @param cmd The parsed command arguments.
 * @return std::vector<std::string> List of numeric errors or replies.
 */
std::vector<std::string> ModeCommand::execute(Client &client, const ParsedCommand &cmd)
{
    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.empty())
    {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, clientNick, "MODE", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    std::string target = cmd.args[0];

    if (target[0] != '#' && target[0] != '&')
        return replies;

    Channel *channel = _server.getChannel(target);
    if (!channel)
    {
        replies.push_back(_server.buildReply(ERR_NOSUCHCHANNEL_CODE, clientNick, target, ERR_NOSUCHCHANNEL_MSG));
        return replies;
    }

    // Scenario 1: User just wants to see the current modes (e.g., "MODE #channel")
    if (cmd.args.size() == 1)
    {
        replies.push_back(_server.buildReply(RPL_CHANNELMODEIS_CODE, clientNick, target, channel->getModes()));
        return replies;
    }

    // Scenario 2: User wants to alter modes. Strict operator validation required.
    if (!channel->isOperator(&client))
    {
        replies.push_back(_server.buildReply(ERR_CHANOPRIVSNEEDED_CODE, clientNick, target, ERR_CHANOPRIVSNEEDED_MSG));
        return replies;
    }

    // Execution phase
    std::string changes = "";
    std::string argsAppended = "";

    parseAndApplyModes(client, channel, cmd, changes, argsAppended);

    // Broadcast the applied changes to the room
    if (!changes.empty())
    {
        std::string modeMsg = ":" + client.getPrefix() + " MODE " + target + " " + changes;
        if (!argsAppended.empty())
            modeMsg += argsAppended;
        channel->broadcast(modeMsg, NULL);
    }

    return replies;
}

/**
 * @brief Parses the mode string character by character and triggers the appropriate handlers.
 * @param client The operator applying the modes.
 * @param channel The channel being modified.
 * @param cmd The full command (needed to extract sequential arguments for k, o, l).
 * @param changes Output reference string accumulating the actual flags modified (e.g., "+k-i").
 * @param argsAppended Output reference string accumulating the parameters of the flags (e.g., " password").
 */
void ModeCommand::parseAndApplyModes(Client &client, Channel *channel, const ParsedCommand &cmd, std::string &changes, std::string &argsAppended)
{
    std::string modeString = cmd.args[1];
    bool adding = true;
    size_t argIndex = 2; // Arguments for k, o, l start at index 2

    for (size_t i = 0; i < modeString.length(); ++i)
    {
        char flag = modeString[i];

        if (flag == '+')
        {
            adding = true;
            if (changes.empty() || changes[changes.length() - 1] != '+')
                changes += "+";
        }
        else if (flag == '-')
        {
            adding = false;
            if (changes.empty() || changes[changes.length() - 1] != '-')
                changes += "-";
        }
        else if (flag == 'i')
        {
            handleInviteMode(channel, adding, changes);
        }
        else if (flag == 't')
        {
            handleTopicMode(channel, adding, changes);
        }
        else if (flag == 'k')
        {
            if (adding && argIndex < cmd.args.size())
            {
                handleKeyMode(channel, adding, cmd.args[argIndex++], changes, argsAppended);
            }
            else if (!adding)
            {
                handleKeyMode(channel, adding, "", changes, argsAppended);
            }
        }
        else if (flag == 'l')
        {
            if (adding && argIndex < cmd.args.size())
            {
                handleLimitMode(channel, adding, cmd.args[argIndex++], changes, argsAppended);
            }
            else if (!adding)
            {
                handleLimitMode(channel, adding, "", changes, argsAppended);
            }
        }
        else if (flag == 'o')
        {
            if (argIndex < cmd.args.size())
            {
                handleOperatorMode(client, channel, adding, cmd.args[argIndex++], changes, argsAppended);
            }
        }
        else
        {
            client.queueMessage(_server.buildReply(ERR_UNKNOWNMODE_CODE, client.getNickname(), std::string(1, flag), BRED "is unknown mode char to me" NC));
        }
    }

    // Clean up trailing +/- if no actual modes were changed (e.g., user sent "+x" which failed)
    if (changes == "+" || changes == "-")
        changes = "";
}

/**
 * @brief Toggles the invite-only status of the channel (+i / -i).
 */
void ModeCommand::handleInviteMode(Channel *channel, bool adding, std::string &changes)
{
    if (channel->isInviteOnly() != adding)
    {
        channel->setInviteOnly(adding);
        changes += "i";
    }
}

/**
 * @brief Toggles the operator-only topic modification restriction (+t / -t).
 */
void ModeCommand::handleTopicMode(Channel *channel, bool adding, std::string &changes)
{
    if (channel->isTopicRestricted() != adding)
    {
        channel->setTopicRestricted(adding);
        changes += "t";
    }
}

/**
 * @brief Sets or removes the channel access password (+k / -k).
 */
void ModeCommand::handleKeyMode(Channel *channel, bool adding, const std::string &arg, std::string &changes, std::string &argsAppended)
{
    if (adding)
    {
        channel->setPassword(arg);
        changes += "k";
        argsAppended += " " + arg;
    }
    else if (channel->hasPassword())
    {
        channel->removePassword();
        changes += "k";
    }
}

/**
 * @brief Sets or removes the maximum user limit of the channel (+l / -l).
 */
void ModeCommand::handleLimitMode(Channel *channel, bool adding, const std::string &arg, std::string &changes, std::string &argsAppended)
{
    if (adding)
    {
        int limit = std::atoi(arg.c_str());
        if (limit > 0)
        {
            channel->setLimit(static_cast<size_t>(limit));
            changes += "l";
            argsAppended += " " + arg;
        }
    }
    else if (channel->hasLimit())
    {
        channel->removeLimit();
        changes += "l";
    }
}

/**
 * @brief Grants or revokes operator status for a specific channel member (+o / -o).
 */
void ModeCommand::handleOperatorMode(Client &client, Channel *channel, bool adding, const std::string &targetNick, std::string &changes, std::string &argsAppended)
{
    (void)client; // Kept in signature for potential future validations
    Client *target = _server.getClientByNickname(targetNick);

    // Silently fail if target doesn't exist or isn't in the channel, as per IRC standard
    if (!target || !channel->isMember(target))
        return;

    if (adding && !channel->isOperator(target))
    {
        channel->addOperator(target);
        changes += "o";
        argsAppended += " " + targetNick;
    }
    else if (!adding && channel->isOperator(target))
    {
        channel->removeOperator(target);
        changes += "o";
        argsAppended += " " + targetNick;
    }
}