/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 15:23:55 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/31 00:05:31 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/QuitCommand.hpp"
// #include "../include/Replies.hpp"
#include <iostream>

QuitCommand::QuitCommand() {}
QuitCommand::~QuitCommand() {}

/**
 * @brief Executes the QUIT command logic.
 * Flags the client for safe disconnection at the end of the server's read cycle
 * to prevent mid-loop memory invalidation (Segfaults).
 * @param client The client requesting to disconnect.
 * @param cmd The parsed command containing an optional quit message.
 * @return An empty vector (replies are handled during the disconnect process).
 */
std::vector<std::string> QuitCommand::execute(Client &client, const ParsedCommand &cmd)
{
    std::vector<std::string> replies;

    std::string reason = cmd.args.empty() ? "Leaving" : cmd.args[0];
    std::string errorMsg = BRED "ERROR :Closing Link: " + client.getNickname() + " (" + reason + ")" NC;
    client.queueMessage(errorMsg);
    client.setPendingDisconnect(true);

    std::cout << BCYN "[QuitCommand] " << client.getNickname() << " is disconnecting." << NC << std::endl;

    return replies;
}