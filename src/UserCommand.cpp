/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:22:58 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 17:23:00 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/UserCommand.hpp"
#include "../include/Replies.hpp"
#include "../include/Server.hpp"
#include <iostream>

UserCommand::UserCommand(Server& server) : _server(server) {}

UserCommand::~UserCommand() {}

/**
 * @brief Executes the USER command logic.
 * Registers the user's details and checks if the full authentication process 
 * (PASS, NICK, and USER) is complete, sending the welcome message if successful.
 * @param client The client attempting to register their user information.
 * @param cmd The parsed command containing the username and realname arguments.
 * @return A vector of formatted IRC replies (errors or welcome message).
 */
std::vector<std::string> UserCommand::execute(Client& client, const ParsedCommand& cmd) {
    
    std::vector<std::string> replies;
    
    // if already registered, cannot change the USER
    if (client.isAuthenticated()) {
        replies.push_back(_server.buildReply(ERR_ALREADYREGISTRED_CODE, "*", ERR_ALREADYREGISTRED_MSG));
        return replies;
    }

    // USER command arguments: <user> <mode> <unused> <realname>
    if (cmd.args.size() < 4) {
        replies.push_back(_server.buildReply(ERR_NEEDMOREPARAMS_CODE, "USER", ERR_NEEDMOREPARAMS_MSG));
        return replies;
    }

    client.setUsername(cmd.args[0]);
    client.setRealname(cmd.args[3]); 

    std::cout << "[UserCommand] Client FD " << client.getFd() << " registered as " << cmd.args[0] << std::endl;

    if (!client.isAuthenticated() && client.hasPassword() && !client.getNickname().empty() && !client.getUsername().empty()) {
        client.setAuthenticated(true);
        replies.push_back(_server.buildReply(RPL_WELCOME_CODE, client.getNickname(), RPL_WELCOME_MSG + client.getNickname()));
        std::cout << BGRN"[Server] Client FD " << client.getFd() << " is now fully authenticated!" << NC << std::endl;
    }
    // IRC Protocol does not require the server to reply in the case that the user is not authenticated correctly
    return replies;
}