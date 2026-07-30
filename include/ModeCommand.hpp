/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:47:31 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 15:47:32 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"
#include <string>

/**
 * @class ModeCommand
 * @brief Parses and executes the IRC MODE command for channels.
 * Manages channel access rules by applying or removing specific flags 
 * (+i, +t, +k, +o, +l) while enforcing strict operator privilege boundaries.
 */
class ModeCommand : public ICommand {
    private:
        Server& _server;

        /**
         * @brief Core parsing engine that iterates through the mode string and delegates actions.
         */
        void parseAndApplyModes(Client& client, Channel* channel, const ParsedCommand& cmd, std::string& changes, std::string& argsAppended);

        // Specific Flag Handlers
        void handleInviteMode(Channel* channel, bool adding, std::string& changes);
        void handleTopicMode(Channel* channel, bool adding, std::string& changes);
        void handleKeyMode(Channel* channel, bool adding, const std::string& arg, std::string& changes, std::string& argsAppended);
        void handleLimitMode(Channel* channel, bool adding, const std::string& arg, std::string& changes, std::string& argsAppended);
        void handleOperatorMode(Client& client, Channel* channel, bool adding, const std::string& targetNick, std::string& changes, std::string& argsAppended);

    public:
        ModeCommand(Server& server);
        ~ModeCommand();

        std::vector<std::string> execute(Client& client, const ParsedCommand& cmd);
};

#endif