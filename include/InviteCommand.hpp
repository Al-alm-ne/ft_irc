/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 10:03:06 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 10:03:11 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITECOMMAND_HPP
#define INVITECOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC INVITE command.
 * Allows channel operators to add users to the channel's invite list and notifies the target.
 */
class InviteCommand : public ICommand {
    private:
        Server& _server;
        void processInvite(Client& client, Client* targetClient, Channel* channel, const std::string& channelName);

    public:
        InviteCommand(Server& server);
        ~InviteCommand();
        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif