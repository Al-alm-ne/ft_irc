/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:58:14 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/16 10:50:10 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Poller.hpp"
#include <cerrno>
#include <stdexcept>

Poller::Poller() {}

Poller::~Poller() {}

void Poller::add(int fd, short events)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = events; // what we want to monitor
	pfd.revents = 0;	 // what actually happened (returned events)
	_fds.push_back(pfd);
}

void Poller::remove(int fd)
{
	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_fds.erase(it);
			return;
		}
	}
}

void Poller::enable(int fd, short event)
{
	for (size_t i = 0; i < _fds.size(); ++i)
	{
		if (_fds[i].fd == fd)
		{
			_fds[i].events |= event; // Use bitwise OR to add the event: if events was POLLIN and event is POLLOUT, becomes POLLIN | POLLOUT. Does not clear other flags, only adds.
			return;
		}
	}
}

void Poller::disable(int fd, short event)
{
	for (size_t i = 0; i < _fds.size(); ++i)
	{
		if (_fds[i].fd == fd)
		{
			_fds[i].events &= ~event; // Use bitwise AND with negation to remove the flag: if events was POLLIN | POLLOUT and event is POLLOUT, ~event clears the POLLOUT bit, leaving only POLLIN
			return;
		}
	}
}

int Poller::wait() // Wait until some FD has an event
{
	if (_fds.empty())
		return 0;
	int result = poll(&_fds[0], _fds.size(), -1);
	if (result < 0) {
		if (errno == EINTR) {
			return 0;
		}
		throw std::runtime_error("poll failed");
	}
	return result;
}

pollfd &Poller::operator[](size_t i)
{
	return _fds[i];
}

size_t Poller::size() const
{
	return _fds.size();
}