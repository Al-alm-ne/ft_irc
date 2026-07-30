/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:56:39 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/16 10:47:57 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <cerrno>

bool Server::_isRunning = true;

Server::Server(int port, const std::string &password)
	: _port(port), _password(password), _serverFd(-1), _name(SERVER_NAME)
{
	initSocket();
}

Server::~Server()
{
	if (_serverFd != -1)
		close(_serverFd);
	std::map<int, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);  // Socket's fd
		delete it->second; // Client's object pointer
	}
	std::map<std::string, Channel*>::iterator it_chan;
	for (it_chan = _channels.begin(); it_chan != _channels.end(); ++it_chan)
	{
		delete it_chan->second;
	}
}

void Server::initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0); // TCP (SOCK_STREAM) IPv4 (AF_INET), 0 = default protocol
	if (_serverFd < 0)
		throw std::runtime_error("Can't creat a socket!");
	// Syscall that sets the properties of a file descriptor. Makes the socket non-blocking
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0) // Set FD flags (F_SETFL) to include O_NONBLOCK
		throw std::runtime_error("Fcntl failed");
	int opt = 1; // Enable the option
	// Syscall that sets special socket options.
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) //(SO_REUSEADDR) Allows reusing the port immediately after the server closes, avoiding "address already in use" errors
		throw std::runtime_error("setsockopt failed");

	sockaddr_in address; // IPv4 address structure, required for bind()
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;				 // IPv4 addresses
	address.sin_addr.s_addr = htonl(INADDR_ANY); // Set IP address
	address.sin_port = htons(_port);			 // Set port

	// Bind the socket (_serverFd) to the IP address + port (addr)
	if (bind(_serverFd, (sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("bind failed");
	// Turn the TCP socket (_serverFd) into a passive socket that listens for client connections (TCP server)
	if (listen(_serverFd, SOMAXCONN) < 0) // Backlog of maximum pending connections before refusing new ones
		throw std::runtime_error("listen failed");
	// Add the server socket FD to the event loop
	poller.add(_serverFd, POLLIN); // (POLLIN) - notify when there's a new connection
	std::cout << BGRN "Server listening on port " << _port << NC << std::endl;
}

/**
 * @brief Intercepts OS signals (like Ctrl+C) to gracefully shut down the server.
 * Sets the static running flag to false, allowing the main poll loop to terminate naturally.
 * @param signum The integer code of the intercepted signal.
 */
void Server::signalHandler(int signum) {
    std::cout << BMAG"\n[Signal " << signum << "] Graceful shutdown initiated. Closing ft_irc..." NC << std::endl;
    Server::_isRunning = false;
}

void Server::runIrc()
{
	Parser parser;
	CommandHandler handler(*this);

	while (Server::_isRunning)
	{
		poller.wait(); // Single poll() of the project. Wait until some FD has an event
		for (size_t i = 0; i < poller.size(); ++i)
		{
			pollfd &pfd = poller[i];
			// If the current FD is _serverFd and revents contains POLLIN, a client is trying to connect
			if (pfd.fd == _serverFd && (pfd.revents & POLLIN))
			{
				acceptNewClient();
				continue; // Skip to the next FD
			}
			if (pfd.revents & POLLIN) // Read - detects data received from a client
				handleRead(pfd.fd, parser, handler);
			if (pfd.revents & POLLOUT) // Write - detects that the socket is ready to send
				handleWrite(pfd.fd);
			if (pfd.revents & (POLLHUP | POLLERR | POLLNVAL)) // Detect errors and disconnections
				disconnectClient(pfd.fd);
		}
	}
}

void Server::acceptNewClient()
{
	while (true)
	{
		int clientFd = accept(_serverFd, NULL, NULL);
		if (clientFd < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			std::cerr << BRED "accept error: " << std::strerror(errno) << NC << std::endl;
			break;
		}
		if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cerr << BRED "fcntl failed on client" << NC << std::endl;
			close(clientFd);
			continue;
		}
		Client *client = new Client(clientFd);
		_clients[clientFd] = client;
		poller.add(clientFd, POLLIN);
		std::cout << BYEL "New client connected: fd=" << BYEL << clientFd << NC << std::endl;
	}
}

void Server::handleRead(int fd, Parser &parser, CommandHandler &handler)
{
	Client *client = _clients[fd];
	char buffer[1024];

	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(fd, buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		disconnectClient(fd);
		return;
	}
	client->appendToRecvBuffer(std::string(buffer, bytes));

	while (client->hasCompleteLine())
	{
		std::string line = client->extractLine();

		//========= Conection with B ============
		ParsedCommand cmd = parser.parse(line); // Component B
		std::vector<std::string> replies = handler.execute(*client, cmd);
		// Returnes a complete formatted response.
		//=======================================

		/*for (size_t i = 0; i < replies.size(); ++i)
			client->queueMessage(replies[i]);
		poller.enable(fd, POLLOUT);*/
		
		// 1. Guarda as respostas no buffer de quem enviou o comando
		for (size_t i = 0; i < replies.size(); ++i) {
			client->queueMessage(replies[i]);
		}
		
		// 2. NOVO: Varre TODOS os clientes e ativa o POLLOUT se tiverem algo para receber
		std::map<int, Client*>::iterator it;
		for (it = _clients.begin(); it != _clients.end(); ++it) {
			if (it->second->hasDataToSend()) {
				poller.enable(it->first, POLLOUT);
			}
		}
	}
}

void Server::handleWrite(int fd)
{
	Client *client = _clients[fd];
	std::string &out = client->getSendBuffer();

	if (out.empty())
	{
		poller.disable(fd, POLLOUT);
		return;
	}
	int bytes = send(fd, out.c_str(), out.size(), 0);
	if (bytes <= 0)
	{
		disconnectClient(fd);
		return;
	}
	out.erase(0, bytes);
	if (out.empty())
		poller.disable(fd, POLLOUT);
	
	// Verifica se o cliente pediu para sair (QUIT)
    if (client->isPendingDisconnect()) {
       	disconnectClient(fd);
    }
}

/**
 * @brief Helper method to handle the business logic of removing a client from all active channels.
 * Broadcasts the QUIT message to peers and safely triggers garbage collection for empty channels.
 * @param client Pointer to the client being disconnected.
 */
void Server::removeClientFromAllChannels(Client* client) {
    std::map<std::string, Channel*>::iterator it = _channels.begin();
    
    while (it != _channels.end()) {
        Channel* chan = it->second;
        if (chan->isMember(client)) {
            chan->broadcast(":" + client->getPrefix() + " QUIT :Client disconnected", NULL);
            chan->removeMember(client);
            chan->removeOperator(client);
        }
        
        if (chan->isEmpty()) {
            delete chan;
            std::map<std::string, Channel*>::iterator toErase = it;
            ++it;
            _channels.erase(toErase);
        } else {
            ++it;
        }
    }
}

/**
 * @brief disconnects a client by closing I/O streams and delegating domain cleanup.
 * @param fd The file descriptor of the client to disconnect.
 */
void Server::disconnectClient(int fd) {
    std::cout << BMAG "Disconnecting client fd=" << fd << NC << std::endl;
    
    std::map<int, Client*>::iterator it = _clients.find(fd);
    if (it != _clients.end()) {
        Client* client = it->second;
        poller.remove(fd);
        removeClientFromAllChannels(client);
        delete client;
        _clients.erase(it);
    }
    close(fd);
}

const std::string& Server::getPassword() const {
    return _password;
}

/**
 * @brief Checks if a specific nickname is currently in use by any connected client.
 * @param nickname The nickname to search for.
 * @return true if the nickname is found, false otherwise.
 */
bool Server::isNicknameInUse(const std::string& nickname) const {
    std::map<int, Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

Channel* Server::getChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        return it->second;
    }
    return NULL;
}

void Server::addChannel(const std::string& name, Channel* channel) {
    _channels[name] = channel;
}

const std::string& Server::getName() const {
    return this->_name;
}

/**
 * @brief Generates a standard RFC 2812 formatted numeric reply.
 * Format: :<server_name> <code> <target> :<message>
 * @param code The 3-digit numeric code (e.g., "403").
 * @param target The recipient's nickname (or "*" if not yet registered).
 * @param msg The human-readable string message.
 * @return The formatted string ready to be queued for the client.
 */
std::string Server::buildReply(const std::string& code, const std::string& target, const std::string& msg) const {
    return ":" + _name + " " + code + " " + target + " :" + msg;
}

/**
 * @brief Generates a complex RFC 2812 formatted numeric reply requiring intermediate arguments.
 * Format: :<server_name> <code> <target> <extraInfo> :<message>
 * @param code The 3-digit numeric code.
 * @param target The recipient's nickname.
 * @param extraInfo Additional data inserted before the final message (e.g., channel name).
 * @param msg The final human-readable string message.
 * @return The formatted string.
 */
std::string Server::buildReply(const std::string& code, const std::string& target, const std::string& extraInfo, const std::string& msg) const {
    return ":" + _name + " " + code + " " + target + " " + extraInfo + " :" + msg;
}

/**
 * @brief Retrieves a connected client by their nickname.
 * @param nickname The exact nickname to search for.
 * @return Pointer to the Client, or NULL if not found.
 */
Client* Server::getClientByNickname(const std::string& nickname) {
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

/**
 * @brief Safely deletes a channel from heap memory and removes it from the server's tracking map.
 * @param name The exact name of the channel to remove.
 */
void Server::removeChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        delete it->second; // Prevents memory leaks
        _channels.erase(it);
    }
}

