/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:14:14 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 10:15:51 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/InviteCommand.hpp"
#include "../include/Replies.hpp"

InviteCommand::InviteCommand(Server& server) : _server(server) {}
InviteCommand::~InviteCommand() {}

/**
 * @brief Executes the INVITE command to add a user to a channel's VIP list.
 * Validates parameter count, channel existence, invoker's membership, and operator privileges.
 * Ensures the target user exists on the server before delegating the state mutation.
 * * @param client The channel operator issuing the INVITE command.
 * @param cmd The parsed command containing the target nickname and the channel name.
 * @return std::vector<std::string> A list of formatted IRC numeric replies (errors) to be sent back to the issuer.
 */
std::vector<std::string> InviteCommand::execute(Client& client, const ParsedCommand& cmd) {
    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.size() < 2) {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, clientNick, "INVITE", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    std::string targetNick = cmd.args[0];
    std::string channelName = cmd.args[1];

    Channel* channel = _server.getChannel(channelName);
    if (!channel) {
        replies.push_back(_server.buildReply(ERR_NOSUCHCHANNEL_CODE, clientNick, channelName, ERR_NOSUCHCHANNEL_MSG));
        return replies;
    }

    if (!channel->isMember(&client)) {
        replies.push_back(_server.buildReply(ERR_NOTONCHANNEL_CODE, clientNick, channelName, ERR_NOTONCHANNEL_MSG));
        return replies;
    }

    if (!channel->isOperator(&client)) {
        replies.push_back(_server.buildReply(ERR_CHANOPRIVSNEEDED_CODE, clientNick, channelName, ERR_CHANOPRIVSNEEDED_MSG));
        return replies;
    }

    Client* targetClient = _server.getClientByNickname(targetNick);
    if (!targetClient) {
        replies.push_back(_server.buildReply(ERR_NOSUCHNICK_CODE, clientNick, targetNick, ERR_NOSUCHNICK_MSG));
        return replies;
    }

    processInvite(client, targetClient, channel, channelName);
    return replies;
}

/**
 * @brief Executes the verified invite action, updates channel state, and routes network notifications.
 * Adds the target user to the channel's internal invite list, sends a direct notification 
 * to the invited user, and returns an acknowledgment numeric to the operator.
 * * @param client The channel operator who authorized the invite.
 * @param targetClient Pointer to the client being invited.
 * @param channel Pointer to the active channel instance.
 * @param channelName The exact name of the channel.
 */
void InviteCommand::processInvite(Client& client, Client* targetClient, Channel* channel, const std::string& channelName) {
    channel->invite(targetClient->getNickname());
    targetClient->queueMessage(":" + client.getPrefix() + " INVITE " + targetClient->getNickname() + " :" + channelName);
    client.queueMessage(_server.buildReply(RPL_INVITING_CODE, client.getNickname(), channelName, targetClient->getNickname()));
}