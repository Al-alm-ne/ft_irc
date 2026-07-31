/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:16:01 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/31 00:26:43 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/KickCommand.hpp"
#include "../include/Replies.hpp"

KickCommand::KickCommand(Server &server) : _server(server) {}
KickCommand::~KickCommand() {}

/**
 * @brief Executes the KICK command to forcefully remove a user from a channel.
 * Validates parameter count, channel existence, invoker's operator status,
 * and ensures the target user is actively present in the channel.
 * * @param client The channel operator issuing the KICK command.
 * @param cmd The parsed command containing the channel name, target nickname, and an optional kick reason.
 * @return std::vector<std::string> A list of formatted IRC numeric replies (errors) to be sent back to the issuer.
 */
std::vector<std::string> KickCommand::execute(Client &client, const ParsedCommand &cmd)
{
    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.size() < 2)
    {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, clientNick, "KICK", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    std::string channelName = cmd.args[0];
    std::string targetNick = cmd.args[1];
    std::string reason = (cmd.args.size() > 2) ? cmd.args[2] : "Kicked by operator";

    Channel *channel = _server.getChannel(channelName);
    if (!channel || !channel->isMember(&client))
    {
        replies.push_back(_server.buildReply(ERR_NOSUCHCHANNEL_CODE, clientNick, channelName, ERR_NOSUCHCHANNEL_MSG));
        return replies;
    }

    if (!channel->isOperator(&client))
    {
        replies.push_back(_server.buildReply(ERR_CHANOPRIVSNEEDED_CODE, clientNick, channelName, ERR_CHANOPRIVSNEEDED_MSG));
        return replies;
    }

    Client *targetClient = _server.getClientByNickname(targetNick);
    if (!targetClient || !channel->isMember(targetClient))
    {
        replies.push_back(_server.buildReply(ERR_USERNOTINCHANNEL_CODE, clientNick, targetNick + " " + channelName, ERR_USERNOTINCHANNEL_MSG));
        return replies;
    }

    processKick(client, targetClient, channel, channelName, reason);
    return replies;
}

/**
 * @brief Executes the verified kick action, handles network broadcasts, and triggers garbage collection.
 * Removes the target from both member and operator lists. If the channel is left empty
 * after the kick, it safely instructs the server to deallocate the channel from heap memory.
 * @param client The channel operator enforcing the kick.
 * @param targetClient Pointer to the client being kicked.
 * @param channel Pointer to the active channel instance.
 * @param channelName The exact name of the channel.
 * @param reason The exit message provided by the operator (or the default reason).
 */
void KickCommand::processKick(Client &client, Client *targetClient, Channel *channel, const std::string &channelName, const std::string &reason)
{
    std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + targetClient->getNickname() + " :" + reason;
    channel->broadcast(kickMsg, NULL);

    channel->removeMember(targetClient);
    channel->removeOperator(targetClient);

    if (channel->isEmpty())
    {
        _server.removeChannel(channelName);
    }
}