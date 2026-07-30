/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:58:07 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/11 15:21:53 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd) : _fd(fd), _isAuthenticated(false), _hasPassword(false), _isPendingDisconnect(false) {}

Client::~Client() {}

int Client::getFd() const
{
	return _fd;
}
// Input: data received from the socket
void Client::appendToRecvBuffer(const std::string &data)
{
	_recvBuffer += data;
}

bool Client::hasCompleteLine() const
{
	return _recvBuffer.find('\n') != std::string::npos; // An IRC line ends with '\n' (may have '\r' before)
}

std::string Client::extractLine()
{
	std::string::size_type pos = _recvBuffer.find('\n');
	std::string line = _recvBuffer.substr(0, pos);		// Extract up to the '\n'
	if (!line.empty() && line[line.size() - 1] == '\r') // Remove '\r' if present
		line.erase(line.size() - 1);
	_recvBuffer.erase(0, pos + 1); // Remove the line from the buffer
	return line;
}
// Output: data to send to the client
void Client::queueMessage(const std::string &msg)
{
	_sendBuffer += msg + "\r\n"; // The IRC protocol requires CRLF
}

bool Client::hasDataToSend() const
{
	return !_sendBuffer.empty();
}

std::string &Client::getSendBuffer()
{
	return _sendBuffer;
}

// ==== Getters & Setters for Client Info ====

bool Client::isAuthenticated() const {
    return _isAuthenticated;
}

void Client::setAuthenticated(bool status) {
    _isAuthenticated = status;
}

bool Client::hasPassword() const {
    return _hasPassword;
}

void Client::setHasPassword(bool status) {
    _hasPassword = status;
}

const std::string& Client::getNickname() const {
    return _nickname;
}

void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

const std::string& Client::getUsername() const {
    return _username;
}

void Client::setUsername(const std::string& username) {
    _username = username;
}

const std::string& Client::getRealname() const {
    return _realname;
}

void Client::setRealname(const std::string& realname) {
    _realname = realname;
}

/**
 * @brief Generates the client's IRC prefix (mask).
 * Format: nickname!username@hostname
 * @return The formatted prefix string.
 */
std::string Client::getPrefix() const {
    std::string host = "127.0.0.1"; // TODO: extract real IP in future implementations
    
    std::string user = _username.empty() ? "user" : _username;
    
    return _nickname + "!" + user + "@" + host;
}

/**
 * @brief Checks if the client has initiated a disconnection (e.g., via QUIT).
 * @return true if a disconnect is pending, false otherwise.
 */
bool Client::isPendingDisconnect() const {
    return _isPendingDisconnect;
}

/**
 * @brief Flags the client for disconnection at the end of the current event loop cycle.
 * @param status The disconnect status to set.
 */
void Client::setPendingDisconnect(bool status) {
    _isPendingDisconnect = status;
}