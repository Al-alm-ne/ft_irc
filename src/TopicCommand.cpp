/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:02:06 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 10:13:59 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/TopicCommand.hpp"
#include "../include/Replies.hpp"

TopicCommand::TopicCommand(Server& server) : _server(server) {}
TopicCommand::~TopicCommand() {}

/**
 * @brief Executes the TOPIC command to view or modify a channel's topic.
 * Performs strict validation on arguments, channel existence, and client membership.
 * Delegates the actual business logic to view or set handlers based on the argument count.
 * * @param client The client (user) issuing the TOPIC command.
 * @param cmd The parsed command containing the target channel and an optional new topic string.
 * @return std::vector<std::string> A list of formatted IRC numeric replies (errors) to be sent back to the issuer.
 */
std::vector<std::string> TopicCommand::execute(Client& client, const ParsedCommand& cmd) {
    std::vector<std::string> replies;
    std::string clientNick = client.getNickname();

    if (cmd.args.empty()) {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, clientNick, "TOPIC", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    std::string channelName = cmd.args[0];
    Channel* channel = _server.getChannel(channelName);

    if (!channel) {
        replies.push_back(_server.buildReply(ERR_NOSUCHCHANNEL_CODE, clientNick, channelName, ERR_NOSUCHCHANNEL_MSG));
        return replies;
    }

    if (!channel->isMember(&client)) {
        replies.push_back(_server.buildReply(ERR_NOTONCHANNEL_CODE, clientNick, channelName, ERR_NOTONCHANNEL_MSG));
        return replies;
    }

    if (cmd.args.size() == 1) {
        handleViewTopic(client, channel, channelName);
    } else {
        handleSetTopic(client, channel, channelName, cmd.args[1]);
    }

    return replies;
}

/**
 * @brief Processes the request to view the current channel topic.
 * Checks if a topic is set and queues the appropriate IRC numeric reply directly to the client's buffer.
 * * @param client The client requesting to view the topic.
 * @param channel Pointer to the active channel instance being queried.
 * @param channelName The exact name of the channel.
 */
void TopicCommand::handleViewTopic(Client& client, Channel* channel, const std::string& channelName) {
    if (channel->getTopic().empty()) {
        client.queueMessage(_server.buildReply(RPL_NOTOPIC_CODE, client.getNickname(), channelName, RPL_NOTOPIC_MSG));
    } else {
        client.queueMessage(_server.buildReply(RPL_TOPIC_CODE, client.getNickname(), channelName, channel->getTopic()));
    }
}

/**
 * @brief Processes the request to update the channel topic and broadcasts the change.
 * Modifies the channel's internal state and notifies all current members of the update.
 * * @param client The client altering the topic.
 * @param channel Pointer to the active channel instance being modified.
 * @param channelName The exact name of the channel.
 * @param newTopic The new topic string to be applied.
 */
void TopicCommand::handleSetTopic(Client& client, Channel* channel, const std::string& channelName, const std::string& newTopic) {
    channel->setTopic(newTopic);
    std::string topicMsg = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic;
    channel->broadcast(topicMsg, NULL);
}