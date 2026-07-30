/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 19:05:44 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/16 10:50:44 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        std::cerr << "Error: invalid number of arguments." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        std::cerr << BRED "Error: port must be a number between 1024 and 65535." << NC << std::endl;
        return 1;
    }

    std::string password = argv[2];
    if (password.empty())
    {
        std::cerr << BRED "Error: password cannot be empty." << NC << std::endl;
        return 1;
    }

    signal(SIGINT, Server::signalHandler);
    signal(SIGQUIT, Server::signalHandler);
    
    std::cout << BYEL "Server starting on port: " << port << NC << std::endl;
    std::cout << BYEL "Password set to: " << password << NC << std::endl;

    try
    {
        Server server(port, password);
        server.runIrc();
    }
    catch (std::exception &e)
    {
        std::cerr << BRED "Fatal error: " << e.what() << NC << "\n";
        return 1;
    }

    return 0;
}