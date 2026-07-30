/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:02:11 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/07 17:02:14 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/NickCommand.hpp"
#include "../include/Server.hpp"
#include "../include/Replies.hpp"
#include <iostream>

NickCommand::NickCommand(Server& server) : _server(server) {}

NickCommand::~NickCommand() {}

/**
 * @brief Validates if the requested nickname contains any illegal characters.
 * @param nick The nickname string to validate.
 * @return true if the nickname is valid, false if it contains forbidden symbols.
 */
static bool isValidNickname(const std::string& nick) {
    if (nick.empty() || nick.find_first_of(" #:,*?!@.") != std::string::npos) {
        return false;
    }
    return true;
}

/**
 * @brief Executes the NICK command logic.
 * Ensures the nickname is provided, valid, and not already in use by another client.
 * Triggers the welcome response if this completes the client's registration flow.
 * @param client The client requesting the nickname change.
 * @param cmd The parsed command containing the new nickname.
 * @return A vector of formatted IRC replies to be sent back to the client.
 */
std::vector<std::string> NickCommand::execute(Client& client, const ParsedCommand& cmd) {
    
    std::vector<std::string> replies;

    if (cmd.args.empty()) {
        replies.push_back(_server.buildReply(ERR_NONICKNAMEGIVEN_CODE, "*", ERR_NONICKNAMEGIVEN_MSG));
        return replies;
    }

    std::string nickname = cmd.args[0];
    if (!isValidNickname(nickname)) {
        replies.push_back(_server.buildReply(ERR_ERRONEUSNICKNAME_CODE, nickname, ERR_ERRONEUSNICKNAME_MSG));
        return replies;
    }
    if (_server.isNicknameInUse(nickname)) {
        replies.push_back(_server.buildReply(ERR_NICKNAMEINUSE_CODE, nickname, ERR_NICKNAMEINUSE_MSG));
        return replies;
    }
    client.setNickname(nickname);
    std::cout << BGRN"[NickCommand] Client FD " << client.getFd() << " set nickname to: " << nickname << NC << std::endl;

    if (!client.isAuthenticated() && client.hasPassword() && !client.getNickname().empty() && !client.getUsername().empty()) {
        client.setAuthenticated(true);
        replies.push_back(_server.buildReply(RPL_WELCOME_CODE, client.getNickname(), RPL_WELCOME_MSG + client.getNickname()));
        std::cout << BMAG"[Server] Client FD " << client.getFd() << " is now fully authenticated!" << NC << std::endl;
    }

    return replies;
}