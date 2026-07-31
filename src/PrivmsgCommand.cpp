/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 12:32:22 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/31 00:02:47 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PrivmsgCommand.hpp"
#include "../include/Replies.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server) : _server(server) {}

PrivmsgCommand::~PrivmsgCommand() {}

/**
 * @brief Executes the PRIVMSG command logic.
 * Validates the arguments and delegates the message delivery to the appropriate handler
 * (Channel or User) based on the target prefix.
 * @param client The client sending the message.
 * @param cmd The parsed command containing the target and the message text.
 * @return A vector of formatted IRC numeric error replies, if any occur.
 */
std::vector<std::string> PrivmsgCommand::execute(Client &client, const ParsedCommand &cmd)
{

    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.empty())
    {
        replies.push_back(_server.buildReply(ERR_NORECIPIENT_CODE, clientNick, ERR_NORECIPIENT_MSG));
        return replies;
    }
    if (cmd.args.size() < 2 || cmd.args[1].empty())
    {
        replies.push_back(_server.buildReply(ERR_NOTEXTTOSEND_CODE, clientNick, ERR_NOTEXTTOSEND_MSG));
        return replies;
    }

    std::string target = cmd.args[0];
    std::string message = cmd.args[1];

    if (target[0] == '#' || target[0] == '&')
    {
        sendToChannel(client, target, message, replies);
    }
    else
    {
        sendToUser(client, target, message, replies);
    }

    return replies;
}

/**
 * @brief Handles the delivery and permissions for a channel message.
 * Checks if the channel exists and if the sender is a member before broadcasting.
 * @param sender The client sending the message.
 * @param target The name of the target channel.
 * @param message The text content to broadcast.
 * @param replies The vector to append error replies to, if any occur.
 */
void PrivmsgCommand::sendToChannel(Client &sender, const std::string &target, const std::string &message, std::vector<std::string> &replies)
{
    std::string clientNick = sender.getNickname();
    Channel *channel = _server.getChannel(target);

    if (!channel)
    {
        replies.push_back(_server.buildReply(ERR_NOSUCHNICK_CODE, clientNick, target, ERR_NOSUCHNICK_MSG));
        return;
    }
    if (!channel->isMember(&sender))
    {
        replies.push_back(_server.buildReply(ERR_CANNOTSENDTOCHAN_CODE, clientNick, target, ERR_CANNOTSENDTOCHAN_MSG));
        return;
    }

    std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + target + " :" + message;
    channel->broadcast(fullMessage, &sender);
    std::cout << BWHT "[PrivmsgCommand] " << clientNick << " sent a message to channel " << target << NC << std::endl;
}

/**
 * @brief Handles the delivery of a direct message (DM) to a specific user.
 * Checks if the target user exists before pushing the message to their socket.
 * @param sender The client sending the message.
 * @param target The nickname of the recipient user.
 * @param message The text content of the direct message.
 * @param replies The vector to append error replies to, if any occur.
 */
void PrivmsgCommand::sendToUser(Client &sender, const std::string &target, const std::string &message, std::vector<std::string> &replies)
{
    std::string clientNick = sender.getNickname();
    Client *targetClient = _server.getClientByNickname(target);

    if (!targetClient)
    {
        replies.push_back(_server.buildReply(ERR_NOSUCHNICK_CODE, clientNick, target, ERR_NOSUCHNICK_MSG));
        return;
    }

    std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + target + " :" + message;
    targetClient->queueMessage(fullMessage);
    std::cout << BWHT "[PrivmsgCommand] " << clientNick << " sent a DM to " << target << NC << std::endl;
}