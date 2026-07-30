/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 09:59:30 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 10:02:47 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPICCOMMAND_HPP
#define TOPICCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC TOPIC command.
 * Allows users to view or modify the channel's topic board.
 */
class TopicCommand : public ICommand {
    private:
        Server& _server;

        void handleViewTopic(Client& client, Channel* channel, const std::string& channelName);
        void handleSetTopic(Client& client, Channel* channel, const std::string& channelName, const std::string& newTopic);

    public:
        TopicCommand(Server& server);
        ~TopicCommand();
        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif