/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:57:51 by alde-alm          #+#    #+#             */
/*   Updated: 2026/07/01 12:57:54 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
#define POLLER_HPP

#include "Irc.hpp"
// Holds all FDs that the server wants to monitor. Calls poll() only once - encapsulates poll()
class Poller
{
private:
	std::vector<pollfd> _fds;

public:
	Poller();
	~Poller();

	void add(int fd, short events); // Add the FD to the event loop to be monitored
	void remove(int fd);
	void enable(int fd, short event);  // Enable a specific event for an FD
	void disable(int fd, short event); // Disable a specific event for an FD
	int wait();						   // Calls the single poll() of the project. Returns number of ready FDs or error

	pollfd &operator[](size_t i); // Operator to access poller[i]
	size_t size() const;		  // Number of pollfd being monitored
};

#endif