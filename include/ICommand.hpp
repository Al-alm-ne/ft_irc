/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 14:18:21 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/03 14:18:23 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <vector>
#include <string>
#include "Client.hpp"
#include "Parser.hpp"

/**
 * @brief Interface for all IRC commands, implementing the Command Design Pattern.
 * By using this interface, the CommandHandler does not need to know the specific 
 * implementation details of each IRC command (e.g., PASS, NICK, USER).
 */
class ICommand {
    public:
        virtual ~ICommand() {}

        /**
        * @brief Executes the specific logic of an IRC command.
        * @param client The client that sent the command. Passed by reference to allow state updates.
        * @param cmd The parsed data transfer object containing the prefix, command, and arguments.
        * @return std::vector<std::string> A list of formatted responses to send back to the client.
        */
        virtual std::vector<std::string> execute(Client& client, const ParsedCommand& cmd) = 0;
};

#endif