/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:25:04 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/06 17:36:43 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <cerrno>

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

void	Server::run()
{
	struct pollfd	pfd;

	pfd.fd = _listenFd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_pollFds.push_back(pfd);

	int	result;
	

	while (true)
	{
		result = poll(&_pollFds[0], _pollFds.size(), -1);
		if (result == -1)
			throw std::runtime_error("Failed to poll server sockets");

		size_t	i;
		size_t	count;

		i = 0;
		count = _pollFds.size();
		while (i < count)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _listenFd)
					acceptClient();
				else
					handleClientRead(_pollFds[i].fd);
			}
			i++;
		}
	}
}
void	Server::acceptClient()
{
	int	clientFd;

	clientFd = accept(_listenFd, NULL, NULL);

	if (clientFd == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			return ;

		throw std::runtime_error("Failed to accept client connection");
	}

	int	result;

	result = fcntl(clientFd, F_SETFL, O_NONBLOCK);

	if (result == -1)
	{
		close(clientFd);
		return ;
	}

	struct pollfd	pfd;

	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_pollFds.push_back(pfd);
}

void	Server::handleClientRead(int clientFd)
{
	(void)clientFd;
}