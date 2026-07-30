/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:03:26 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 10:03:32 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC KICK command.
 * Enforces operator privileges to forcibly remove a user from a channel.
 */
class KickCommand : public ICommand {
    private:
        Server& _server;
        void processKick(Client& client, Client* targetClient, Channel* channel, const std::string& channelName, const std::string& reason);

    public:
        KickCommand(Server& server);
        ~KickCommand();
        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif