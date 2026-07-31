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

/**
 * @brief Constructs a client object for a given socket descriptor.
 * Initializes authentication, password, and disconnect state to their default values.
 * @param fd The connected socket file descriptor associated with this client.
 */
Client::Client(int fd) : _fd(fd), _isAuthenticated(false), _hasPassword(false), _isPendingDisconnect(false) {}

/**
 * @brief Destroys the client instance.
 */
Client::~Client() {}

/**
 * @brief Retrieves the client's socket file descriptor.
 * @return The connected socket descriptor.
 */
int Client::getFd() const
{
	return _fd;
}

/**
 * @brief Appends incoming socket data to the internal receive buffer.
 * @param data Raw bytes received from the network.
 */
void Client::appendToRecvBuffer(const std::string &data)
{
	_recvBuffer += data;
}

/**
 * @brief Checks whether the receive buffer already contains a full IRC line.
 * @return true if at least one line terminator is present, false otherwise.
 */
bool Client::hasCompleteLine() const
{
	return _recvBuffer.find('\n') != std::string::npos; // An IRC line ends with '\n' (may have '\r' before)
}

/**
 * @brief Extracts the next complete IRC line from the receive buffer.
 * Removes a trailing carriage return if present and consumes the line from the buffer.
 * @return The extracted IRC line without the line ending.
 */
std::string Client::extractLine()
{
	std::string::size_type pos = _recvBuffer.find('\n');
	std::string line = _recvBuffer.substr(0, pos);		// Extract up to the '\n'
	if (!line.empty() && line[line.size() - 1] == '\r') // Remove '\r' if present
		line.erase(line.size() - 1);
	_recvBuffer.erase(0, pos + 1); // Remove the line from the buffer
	return line;
}

/**
 * @brief Queues a message to be sent to the client.
 * Appends IRC CRLF line termination automatically.
 * @param msg The IRC-formatted message to enqueue.
 */
void Client::queueMessage(const std::string &msg)
{
	_sendBuffer += msg + "\r\n"; // The IRC protocol requires CRLF
}

/**
 * @brief Checks whether the client has pending outbound data.
 * @return true if the send buffer is not empty, false otherwise.
 */
bool Client::hasDataToSend() const
{
	return !_sendBuffer.empty();
}

/**
 * @brief Provides mutable access to the client's send buffer.
 * @return A reference to the pending outbound data buffer.
 */
std::string &Client::getSendBuffer()
{
	return _sendBuffer;
}

/**
 * @brief Checks whether the client has completed authentication.
 * @return true if the client is fully authenticated, false otherwise.
 */
bool Client::isAuthenticated() const {
    return _isAuthenticated;
}

/**
 * @brief Updates the client's authentication flag.
 * @param status New authentication state.
 */
void Client::setAuthenticated(bool status) {
    _isAuthenticated = status;
}

/**
 * @brief Checks whether the client has already provided the server password.
 * @return true if the password step succeeded, false otherwise.
 */
bool Client::hasPassword() const {
    return _hasPassword;
}

/**
 * @brief Updates the client's password verification flag.
 * @param status New password verification state.
 */
void Client::setHasPassword(bool status) {
    _hasPassword = status;
}

/**
 * @brief Retrieves the client's nickname.
 * @return The current nickname string.
 */
const std::string& Client::getNickname() const {
    return _nickname;
}

/**
 * @brief Sets the client's nickname.
 * @param nickname The nickname to assign.
 */
void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

/**
 * @brief Retrieves the client's username.
 * @return The current username string.
 */
const std::string& Client::getUsername() const {
    return _username;
}

/**
 * @brief Sets the client's username.
 * @param username The username to assign.
 */
void Client::setUsername(const std::string& username) {
    _username = username;
}

/**
 * @brief Retrieves the client's real name.
 * @return The current real-name string.
 */
const std::string& Client::getRealname() const {
    return _realname;
}

/**
 * @brief Sets the client's real name.
 * @param realname The real name to assign.
 */
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