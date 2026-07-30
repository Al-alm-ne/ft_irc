/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:21:22 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/11 15:21:29 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "ICommand.hpp"
// #include "Server.hpp"

/**
 * @brief Handles the IRC QUIT command.
 * Initiates the safe disconnection process for a client.
 * Flags the client to be removed from all channels and disconnected by the Server
 * at the end of the current read cycle to prevent memory invalidation.
 */
class QuitCommand : public ICommand {
    // private:
    //     Server& _server;

    public:
        QuitCommand();
        ~QuitCommand();

        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif