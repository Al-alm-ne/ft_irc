/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 14:18:25 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 17:01:12 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

# include <map>
# include <string>
# include <vector>
# include "ICommand.hpp"
# include "Client.hpp"
# include "Parser.hpp"

// Forward declaration to avoid circular dependency with Server.hpp
class Server;

/**
 * @brief Acts as a central router for incoming IRC commands.
 * Maps string identifiers (like "JOIN" or "PASS") to their respective ICommand implementations.
 */
class CommandHandler {
private:
    Server& _server;
    std::map<std::string, ICommand*> _commands;

public:
    CommandHandler(Server& server);
    ~CommandHandler();

    std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif