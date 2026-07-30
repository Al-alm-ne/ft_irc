/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:57:59 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/16 10:47:19 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// # include "Irc.hpp"
# include "Poller.hpp"
# include "Client.hpp"
# include "Parser.hpp"
# include "CommandHandler.hpp"
# include "Channel.hpp"
# include <csignal>

# define SERVER_NAME "irc.42porto.com"

class Server
{
	private:
		int _port;
		std::string _password;
		int _serverFd;
		std::string _name;
		
		Poller poller;					  // Integrated Poller - manages all FDs
		std::map<int, Client *> _clients; // map of fd to Client pointer
		std::map<std::string, Channel*> _channels;
		
		void initSocket();												 // Create, configure and put the socket (FD) into listen
		void acceptNewClient();											 // Accept new connections
		void handleRead(int fd, Parser &parser, CommandHandler &handle); // Handle reading data
		void handleWrite(int fd);										 // Handle sending data
		void disconnectClient(int fd);
		void removeClientFromAllChannels(Client* client);									 // Remove clients, close FD and clean up
		
		static bool _isRunning;

	public:
		Server(int port, const std::string &password);
		~Server();

		void runIrc();

		const std::string& getPassword() const;
		bool isNicknameInUse(const std::string& nickname) const;
		Channel* getChannel(const std::string& name);
		void addChannel(const std::string& name, Channel* channel);
		const std::string& getName() const;

		// ==== Reply Builders ====
		std::string buildReply(const std::string& code, const std::string& target, const std::string& msg) const;
		std::string buildReply(const std::string& code, const std::string& target, const std::string& extraInfo, const std::string& msg) const;

		// === for PRIVMSG cmd === 
		Client* getClientByNickname(const std::string& nickname);

		//=== for part & quit ===
		void removeChannel(const std::string& name);

		static void signalHandler(int signum);
};	

#endif