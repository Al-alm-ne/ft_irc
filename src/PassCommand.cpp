/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 14:42:42 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 15:30:38 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PassCommand.hpp"
#include "../include/Server.hpp"
#include "../include/Replies.hpp"
#include <iostream>

/**
 * @brief Constructs the PassCommand with a reference to the Server.
 * @param server The main server instance, used to validate the connection password.
 */
PassCommand::PassCommand(Server& server) : _server(server) {}

/**
 * @brief Destructor for PassCommand.
 */
PassCommand::~PassCommand() {}

/**
 * @brief Executes the PASS command logic.
 * * Validates the password provided by the client against the server's password.
 * According to RFC 2812:
 * - ERR_NEEDMOREPARAMS (461) if no password is provided.
 * - ERR_ALREADYREGISTRED (462) if the client is already authenticated.
 * - ERR_PASSWDMISMATCH (464) if the password is incorrect.
 * * @param client The client attempting to set the password.
 * @param cmd The parsed command containing the arguments.
 * @return std::vector<std::string> A list of IRC numeric replies if errors occur.
 */
std::vector<std::string> PassCommand::execute(Client& client, const ParsedCommand& cmd) {
    std::vector<std::string> replies;

    if (client.isAuthenticated()) {
        replies.push_back(_server.buildReply(ERR_ALREADYREGISTRED_CODE, "*", ERR_ALREADYREGISTRED_MSG));
        return replies;
    }

    if (cmd.args.empty()) {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, "PASS", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    if (cmd.args[0] == _server.getPassword()) {
        client.setHasPassword(true);
        std::cout << BGRN"[PassCommand] Client FD " << client.getFd() << " provided the correct password." << NC << std::endl;
    } else {
        replies.push_back(_server.buildReply(ERR_PASSWDMISMATCH_CODE, "*", ERR_PASSWDMISMATCH_MSG));
        std::cout << BRED"[PassCommand] Client FD " << client.getFd() << " provided an incorrect password." << NC << std::endl;
    }

    return replies;
}