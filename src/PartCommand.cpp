/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:23:52 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/08/01 22:41:56 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PartCommand.hpp"
#include "../include/Replies.hpp"
#include <iostream>

#include "../include/PartCommand.hpp"
#include "../include/Replies.hpp"
#include <iostream>

PartCommand::PartCommand(Server &server) : _server(server) {}
PartCommand::~PartCommand() {}

/**
 * @brief Executes the PART command validation routing.
 * @param client The client attempting to leave the channel.
 * @param cmd The parsed command containing the channel name and optional reason.
 * @return A vector of formatted IRC numeric error replies, if any occur.
 */
std::vector<std::string> PartCommand::execute(Client &client, const ParsedCommand &cmd)
{
    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.empty())
    {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, clientNick, "PART", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    std::string channelName = cmd.args[0];
    std::string reason = (cmd.args.size() > 1) ? cmd.args[1] : BMAG "Leaving" NC;

    Channel *channel = _server.getChannel(channelName);
    if (!channel)
    {
        replies.push_back(_server.buildReply(ERR_NOSUCHCHANNEL_CODE, clientNick, channelName, ERR_NOSUCHCHANNEL_MSG));
        return replies;
    }

    if (!channel->isMember(&client))
    {
        replies.push_back(_server.buildReply(ERR_NOTONCHANNEL_CODE, clientNick, channelName, ERR_NOTONCHANNEL_MSG));
        return replies;
    }

    processPart(client, channel, channelName, reason);

    return replies;
}

/**
 * @brief Handles the business logic of a validated PART command.
 * Broadcasts the departure, updates channel state, and triggers garbage collection.
 * @param client The client leaving the channel.
 * @param channel Pointer to the active channel instance.
 * @param channelName The exact name of the channel.
 * @param reason The exit message provided by the user (or default).
 */
void PartCommand::processPart(Client &client, Channel *channel, const std::string &channelName, const std::string &reason)
{
    std::string partMsg = ":" + client.getPrefix() + " PART " + channelName + " :" + reason;
    channel->broadcast(partMsg, NULL);

    channel->removeMember(&client);
    channel->removeOperator(&client);

    std::cout << BMAG "[PartCommand] " << client.getNickname() << " left " << channelName << NC << std::endl;

    if (channel->isEmpty())
    {
        _server.removeChannel(channelName);
        std::cout << BBLU "[Server] Channel " << channelName << " deleted (empty)." << NC << std::endl;
    }
}