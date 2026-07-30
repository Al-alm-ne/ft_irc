/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:21:09 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/11 15:37:08 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC PART command.
 * Allows a client to leave a specific channel, notifying other members.
 * Responsible for triggering channel garbage collection if the channel becomes empty.
 */
class PartCommand : public ICommand {
    private:
        Server& _server;

        void processPart(Client& client, Channel* channel, const std::string& channelName, const std::string& reason);

    public:
        PartCommand(Server& server);
        ~PartCommand();

        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif