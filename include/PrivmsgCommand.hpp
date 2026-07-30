/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivmsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 12:32:10 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/11 12:40:00 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSGCOMMAND_HPP
#define PRIVMSGCOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC PRIVMSG command.
 * Responsible for routing messages to specific channels (broadcasting) or 
 * directly to specific users (Direct Messages).
 */
class PrivmsgCommand : public ICommand {
    private:
        Server& _server;

        void sendToChannel(Client& sender, const std::string& target, const std::string& message, std::vector<std::string>& replies);
        void sendToUser(Client& sender, const std::string& target, const std::string& message, std::vector<std::string>& replies);

    public:
        PrivmsgCommand(Server& server);
        ~PrivmsgCommand();

        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif