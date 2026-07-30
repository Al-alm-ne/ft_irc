/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:54:37 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/03 11:54:38 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

// === DTO (Data Transfer Object) ===
struct ParsedCommand {
    std::string prefix;
    std::string command;
    std::vector<std::string> args;
};

class Parser {
private:
    void skipSpaces(std::string& line) const;
    void parsePrefix(std::string& line, ParsedCommand& cmd) const;
    void parseCommand(std::string& line, ParsedCommand& cmd) const;
    void parseArguments(std::string& line, ParsedCommand& cmd) const;

public:
    Parser();
    ~Parser();

    ParsedCommand parse(const std::string& line) const;
};

#endif