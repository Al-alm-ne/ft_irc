/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:59:29 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 16:59:33 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICKCOMMAND_HPP
#define NICKCOMMAND_HPP

#include "ICommand.hpp"

class Server;

/**
 * @brief Handles the IRC NICK command.
 * Responsible for validating, setting, or changing a client's nickname, handling collisions.
 */
class NickCommand : public ICommand {
private:
    Server& _server;

public:
    NickCommand(Server& server);
    ~NickCommand();

    std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif