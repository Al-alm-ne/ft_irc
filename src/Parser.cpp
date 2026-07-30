/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:54:26 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/03 12:01:21 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

/**
 * @brief Orchestrates the parsing of a raw IRC message into a structured DTO.
 * * @param line The raw string received from the network (without \r\n characters).
 * @return ParsedCommand The structured object containing the prefix, command, and arguments.
 */
ParsedCommand Parser::parse(const std::string& line) const {
    ParsedCommand cmd;
    std::string currentLine = line;

    skipSpaces(currentLine);
    parsePrefix(currentLine, cmd);
    skipSpaces(currentLine);
    parseCommand(currentLine, cmd);
    skipSpaces(currentLine);
    parseArguments(currentLine, cmd);

    return cmd;
}

/**
 * @brief Removes consecutive spaces at the beginning of the string.
 * * @param line A reference to the string that will be modified in place.
 */
void Parser::skipSpaces(std::string& line) const {
    while (!line.empty() && line[0] == ' ') {
        line.erase(0, 1);
    }
}

/**
 * @brief Extracts the prefix from the message if it starts with a ':' character.
 * * @param line The string being processed (consumed in real-time).
 * @param cmd The DTO where the extracted prefix will be stored.
 */
void Parser::parsePrefix(std::string& line, ParsedCommand& cmd) const {
    if (line.empty() || line[0] != ':')
        return;

    std::string::size_type pos = line.find(' ');
    if (pos != std::string::npos) {
        cmd.prefix = line.substr(1, pos - 1); // Ignore the starting ':'
        line.erase(0, pos + 1);
    } else {
        cmd.prefix = line.substr(1);
        line.clear();
    }
}

/**
 * @brief Extracts the main command word from the string.
 * * @param line The string being processed (consumed in real-time).
 * @param cmd The DTO where the extracted command will be stored.
 */
void Parser::parseCommand(std::string& line, ParsedCommand& cmd) const {
    if (line.empty())
        return;

    std::string::size_type pos = line.find(' ');
    if (pos != std::string::npos) {
        cmd.command = line.substr(0, pos);
        line.erase(0, pos + 1);
    } else {
        cmd.command = line;
        line.clear();
    }
}

/**
 * @brief Extracts all subsequent arguments, handling the 'trailing parameter' rule.
 * * @param line The string being processed (consumed in real-time).
 * @param cmd The DTO where the arguments list will be populated.
 */
void Parser::parseArguments(std::string& line, ParsedCommand& cmd) const {
    while (!line.empty()) {
        skipSpaces(line);
        if (line.empty())
            break;

        // If a trailing parameter is found (starts with ':')
        if (line[0] == ':') {
            cmd.args.push_back(line.substr(1)); // Consume the rest of the string
            break;
        }
        
        // If it's a standard argument separated by spaces
        std::string::size_type pos = line.find(' ');
        if (pos != std::string::npos) {
            cmd.args.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        } else {
            cmd.args.push_back(line);
            break;
        }
    }
}