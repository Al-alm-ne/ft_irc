/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:57:24 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/11 15:16:46 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

/**
 * @brief Represents a connected network client in the IRC server.
 * Manages the client's socket file descriptor, I/O buffers, and authentication state.
 */
class Client
{
private:
	int _fd;
	std::string _recvBuffer;
	std::string _sendBuffer;

	bool _isAuthenticated;
	bool _hasPassword;
	std::string _nickname;
    std::string _username;
    std::string _realname;

	bool _isPendingDisconnect;

public:
	Client(int fd);
	~Client();

	int getFd() const;

	// ======== Input ========
	void appendToRecvBuffer(const std::string &data);
	bool hasCompleteLine() const;
	std::string extractLine();

	// ======== Output ========
	void queueMessage(const std::string &msg);
	bool hasDataToSend() const;
	std::string &getSendBuffer();

	// === Getters & Setters ===
	bool isAuthenticated() const;
    void setAuthenticated(bool status);

    bool hasPassword() const;
    void setHasPassword(bool status);

    const std::string& getNickname() const;
    void setNickname(const std::string& nickname);

    const std::string& getUsername() const;
    void setUsername(const std::string& username);

    const std::string& getRealname() const;
    void setRealname(const std::string& realname);

	std::string getPrefix() const;

	bool isPendingDisconnect() const;
	void setPendingDisconnect(bool status);
};

#endif