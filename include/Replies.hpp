/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:22:27 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 14:54:49 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "Colors.hpp"

/**
 * @file Replies.hpp
 * @brief Defines standard IRC numeric reply codes and messages according to RFC 2812.
 * Serves as a single source of truth for protocol-specific static strings.
 */
 
#define ERR_NEEDMOREPARAMS_CODE "461"
#define ERR_NEEDMOREPARAMS_MSG BRED"Not enough parameters" NC

#define ERR_ALREADYREGISTRED_CODE "462"
#define ERR_ALREADYREGISTRED_MSG BRED"Unauthorized command (already registered)" NC

#define ERR_PASSWDMISMATCH_CODE "464"
#define ERR_PASSWDMISMATCH_MSG BRED"Password incorrect" NC

#define ERR_UNKNOWNCOMMAND_CODE "421"
#define ERR_UNKNOWNCOMMAND_MSG BRED"Unknown command" NC

// === NICK ====
#define ERR_NONICKNAMEGIVEN_CODE "431"
#define ERR_NONICKNAMEGIVEN_MSG BRED"No nickname given" NC

#define ERR_ERRONEUSNICKNAME_CODE "432"
#define ERR_ERRONEUSNICKNAME_MSG BRED"Erroneous nickname" NC

#define ERR_NICKNAMEINUSE_CODE "433"
#define ERR_NICKNAMEINUSE_MSG BRED"Nickname is already in use" NC

// === welcome message === 

#define RPL_WELCOME_CODE "001"
#define RPL_WELCOME_MSG BYEL"Welcome to the ft_irc Network, " NC

// === JOIN ====
#define ERR_NOSUCHCHANNEL_CODE "403"
#define ERR_NOSUCHCHANNEL_MSG BRED"No such channel" NC

#define RPL_NAMREPLY_CODE "353"

#define RPL_ENDOFNAMES_CODE "366"
#define RPL_ENDOFNAMES_MSG BGRN"End of /NAMES list" NC

// === unauth error ===
#define ERR_NOTREGISTERED_CODE "451"
#define ERR_NOTREGISTERED_MSG BRED"You have not registered" NC

// === PRIVMSG ====
#define ERR_NOSUCHNICK_CODE "401"
#define ERR_NOSUCHNICK_MSG BRED"No such nick/channel" NC

#define ERR_CANNOTSENDTOCHAN_CODE "404"
#define ERR_CANNOTSENDTOCHAN_MSG BRED"Cannot send to channel" NC

#define ERR_NORECIPIENT_CODE "411"
#define ERR_NORECIPIENT_MSG BRED"No recipient given (PRIVMSG)" NC

#define ERR_NOTEXTTOSEND_CODE "412"
#define ERR_NOTEXTTOSEND_MSG BRED"No text to send" NC

// === PART ====
#define ERR_NOTONCHANNEL_CODE "442"
#define ERR_NOTONCHANNEL_MSG BRED"You're not on that channel" NC

// === TOPIC ===

#define RPL_NOTOPIC_CODE "331"
#define RPL_NOTOPIC_MSG BRED"No topic is set" NC
#define RPL_TOPIC_CODE "332"
#define RPL_INVITING_CODE "341"

#define ERR_USERNOTINCHANNEL_CODE "441"
#define ERR_USERNOTINCHANNEL_MSG BRED"They aren't on that channel" NC
#define ERR_CHANOPRIVSNEEDED_CODE "482"
#define ERR_CHANOPRIVSNEEDED_MSG BRED"You're not channel operator" NC

// === MODE ===
#define RPL_CHANNELMODEIS_CODE "324"

#define ERR_CHANNELISFULL_CODE "471"
#define ERR_CHANNELISFULL_MSG BRED"Cannot join channel (+l) - channel is full" NC
#define ERR_UNKNOWNMODE_CODE "472"
#define ERR_INVITEONLYCHAN_CODE "473"
#define ERR_INVITEONLYCHAN_MSG BRED"Cannot join channel (+i) - you must be invited" NC
#define ERR_BADCHANNELKEY_CODE "475"
#define ERR_BADCHANNELKEY_MSG BRED"Cannot join channel (+k) - bad key" NC

#endif