/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:40:19 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 14:57:42 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) 
    : _name(name), _topic(""), _inviteOnly(false), _topicRestricted(false), _userLimit(0) {}

Channel::~Channel() {}

/**
 * @brief Gets the channel's name.
 * @return The name of the channel.
 */
const std::string& Channel::getName() const {
    return _name;
}

/**
 * @brief Gets the channel's current topic.
 * @return The topic string.
 */
const std::string& Channel::getTopic() const {
    return _topic;
}

/**
 * @brief Sets or updates the channel's topic.
 * @param topic The new topic string.
 */
void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

/**
 * @brief Checks if a client is present in the channel.
 * @param client Pointer to the client to check.
 * @return true if the client is a member, false otherwise.
 */
bool Channel::isMember(Client* client) const {
    return std::find(_members.begin(), _members.end(), client) != _members.end();
}

/**
 * @brief Adds a client to the channel if not already a member.
 * @param client Pointer to the client to add.
 */
void Channel::addMember(Client* client) {
    if (!isMember(client)) {
        _members.push_back(client);
    }
}

/**
 * @brief Checks if a client has operator privileges in the channel.
 * @param client Pointer to the client to check.
 * @return true if the client is an operator, false otherwise.
 */
bool Channel::isOperator(Client* client) const {
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

/**
 * @brief Grants operator privileges to a client.
 * @param client Pointer to the client to promote.
 */
void Channel::addOperator(Client* client) {
    if (!isOperator(client)) {
        _operators.push_back(client);
    }
}

/**
 * @brief Retrieves the list of all members in the channel.
 * @return A constant reference to the vector of member clients.
 */
const std::vector<Client*>& Channel::getMembers() const {
    return _members;
}

/**
 * @brief Retrieves the list of all operators in the channel.
 * @return A constant reference to the vector of operator clients.
 */
const std::vector<Client*>& Channel::getOperators() const {
    return _operators;
}

/**
 * @brief Broadcasts a message to all members of the channel, except the sender.
 * @param message The fully formatted IRC message string.
 * @param sender Pointer to the client who originated the message.
 */
void Channel::broadcast(const std::string& message, Client* sender) {
    for (size_t i = 0; i < _members.size(); ++i) {
        if (_members[i] != sender) {
            _members[i]->queueMessage(message);
        }
    }
}

/**
 * @brief Removes a client from the channel's member list.
 * @param client Pointer to the client to be removed.
 */
void Channel::removeMember(Client* client) {
    std::vector<Client*>::iterator it = std::find(_members.begin(), _members.end(), client);
    if (it != _members.end()) {
        _members.erase(it);
    }
}

/**
 * @brief Removes a client from the channel's operator list.
 * @param client Pointer to the client to be removed from operators.
 */
void Channel::removeOperator(Client* client) {
    std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end()) {
        _operators.erase(it);
    }
}

/**
 * @brief Checks if the channel has no members left.
 * Useful for triggering channel destruction to prevent memory leaks.
 * @return true if the channel is empty, false otherwise.
 */
bool Channel::isEmpty() const {
    return _members.empty();
}

/**
* @brief Adds a nickname to the channel's VIP invite list.
* @param nickname The exact nickname of the invited user.
*/
void Channel::invite(const std::string& nickname) {
    if (!isInvited(nickname))
        _invitedNicks.push_back(nickname);
}

/**
* @brief Verifies if a nickname is explicitly invited to the channel.
* @param nickname The nickname to check.
* @return true if the user is on the invite list, false otherwise.
*/
bool Channel::isInvited(const std::string& nickname) const {
    return std::find(_invitedNicks.begin(), _invitedNicks.end(), nickname) != _invitedNicks.end();
}

bool Channel::isInviteOnly() const { 
    return _inviteOnly; 
}

void Channel::setInviteOnly(bool state) { 
    _inviteOnly = state; 
}

bool Channel::isTopicRestricted() const { 
    return _topicRestricted; 
}

void Channel::setTopicRestricted(bool state) { 
    _topicRestricted = state; 
}

bool Channel::hasPassword() const { 
    return !_password.empty(); 
}

const std::string& Channel::getPassword() const { 
    return _password; 
}

void Channel::setPassword(const std::string& key) { 
    _password = key; 
}

void Channel::removePassword() { 
    _password.clear(); 
}

bool Channel::hasLimit() const { 
    return _userLimit > 0; 
}

size_t Channel::getLimit() const { 
    return _userLimit; 
}

void Channel::setLimit(size_t limit) {
    _userLimit = limit; 
}

void Channel::removeLimit() { 
    _userLimit = 0;
}

size_t Channel::getMemberCount() const { 
    return _members.size(); 
}

/**
 * @brief Constructs a string representing the currently active modes.
 * Used for responding to empty MODE commands.
 * @return A string formatted as "+[modes]" (e.g., "+itk").
 */
std::string Channel::getModes() const {
    std::string modes = "+";
    if (_inviteOnly) modes += "i";
    if (_topicRestricted) modes += "t";
    if (hasPassword()) modes += "k";
    if (hasLimit()) modes += "l";
    return modes;
}