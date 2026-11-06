/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:25:04 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/06 16:12:35 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>

Server::Server(int port, const std::string &password)
	: _listenFd(-1), _port(port), _password(password)
{
	initSocket();
}

Server::~Server()
{
	closeSocket();
}

void	Server::initSocket()
{
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);

	if (_listenFd == -1)
		throw std::runtime_error("Failed to create server socket");

	struct sockaddr_in	addr;

	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int	result;

	result = fcntl(_listenFd, F_SETFL, O_NONBLOCK);

	if (result == -1)
	{
		closeSocket();
		throw std::runtime_error("Failed to set server socket to non-blocking mode");
	}

	result = bind(_listenFd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));

	if (result == -1)
	{
		closeSocket();
		throw std::runtime_error("Failed to bind server socket");
	}

	result = listen(_listenFd, SOMAXCONN);

	if (result == -1)
	{
		closeSocket();
		throw std::runtime_error("Failed to listen on server socket");
	}
}

void	Server::closeSocket()
{
	if (_listenFd != -1)
		close(_listenFd);
	
	_listenFd = -1;
}