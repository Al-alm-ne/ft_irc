/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 14:19:34 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 15:49:52 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CommandHandler.hpp"
#include "../include/Server.hpp"
#include "../include/Replies.hpp"
#include "../include/PassCommand.hpp"
#include "../include/UserCommand.hpp"
#include "../include/NickCommand.hpp"
#include "../include/JoinCommand.hpp"
#include "../include/PrivmsgCommand.hpp"
#include "../include/QuitCommand.hpp"
#include "../include/PartCommand.hpp"
#include "../include/TopicCommand.hpp"
#include "../include/InviteCommand.hpp"
#include "../include/KickCommand.hpp"
#include "../include/ModeCommand.hpp"
#include <iostream>

/**
 * @brief Constructs the CommandHandler and initializes the command registry.
 * @param server Reference to the main IRC server to access global configurations (e.g., password).
 */
CommandHandler::CommandHandler(Server& server) : _server(server) {
    _commands["PASS"] = new PassCommand(server);
    _commands["USER"] = new UserCommand(server);
    _commands["NICK"] = new NickCommand(server);
    _commands["JOIN"] = new JoinCommand(server);
    _commands["PRIVMSG"] = new PrivmsgCommand(server);
    _commands["QUIT"] = new QuitCommand();
    _commands["PART"] = new PartCommand(server);
    _commands["TOPIC"] = new TopicCommand(server);
    _commands["INVITE"] = new InviteCommand(server);
    _commands["KICK"] = new KickCommand(server);
    _commands["MODE"] = new ModeCommand(server);
}

/**
 * @brief Destructor that cleans up dynamically allocated command objects from the heap.
 */
CommandHandler::~CommandHandler() {
    std::map<std::string, ICommand*>::iterator it;
    for (it = _commands.begin(); it != _commands.end(); ++it) {
        delete it->second;
    }
    _commands.clear();
}

/**
 * @brief Routes the parsed command to the appropriate command execution class.
 * @param client The user who triggered the command.
 * @param cmd The parsed data from the network.
 * @return std::vector<std::string> The generated IRC responses to queue for the client.
 */
std::vector<std::string> CommandHandler::execute(Client& client, const ParsedCommand& cmd) {
    std::vector<std::string> replies;

    if (cmd.command.empty()) {
        return replies;
    }

    if (!client.isAuthenticated() && cmd.command != "PASS" && cmd.command != "NICK" && cmd.command != "USER" && cmd.command != "QUIT") {
        replies.push_back(_server.buildReply(ERR_NOTREGISTERED_CODE, client.getNickname().empty() ? "*" : client.getNickname(), ERR_NOTREGISTERED_MSG));
        std::cout << BRED"[Security] Blocked unauthenticated command: " << cmd.command << " from FD " << client.getFd() << NC << std::endl;
        return replies;
    }
    
    std::map<std::string, ICommand*>::iterator it = _commands.find(cmd.command);
    
    if (it != _commands.end()) {
        replies = it->second->execute(client, cmd);
    } else {
        replies.push_back(_server.buildReply(ERR_UNKNOWNCOMMAND_CODE, cmd.command, ERR_UNKNOWNCOMMAND_MSG));
        std::cout << BRED"[CommandHandler] Unknown command received: " << cmd.command << NC << std::endl;
    }

    return replies;
}