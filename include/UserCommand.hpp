/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:40:14 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 17:40:16 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include "ICommand.hpp"
#include "Server.hpp"

/**
 * @brief Handles the IRC USER command.
 * Completes the client registration process by setting their username and real name.
 */
class UserCommand : public ICommand {
    private:
        Server& _server;

    public:
        UserCommand(Server& server);
        ~UserCommand();

        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif