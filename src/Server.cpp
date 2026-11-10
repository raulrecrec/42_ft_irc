/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:25:04 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/10 21:53:15 by rexposit         ###   ########.fr       */
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
#include "Client.hpp"

//DEBUG
#define DEBUG 1
#include <iostream>

Server::Server(int port, const std::string &password)
	: _listenFd(-1), _port(port), _password(password)
{
	initSocket();
}

Server::~Server()
{
	while (!_clients.empty())
	{
		removeClient(_clients.begin()->first);
	}
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
		{
			if (errno == EINTR)
				continue ;
			throw std::runtime_error("Failed to poll server sockets");
		}

		size_t	i;
		size_t	count;
		size_t	previousSize;

		i = 0;
		count = _pollFds.size();
		while (i < count)
		{
			if (_pollFds[i].fd == _listenFd)
			{
				if (_pollFds[i].revents & (POLLERR | POLLNVAL | POLLHUP))
					throw std::runtime_error("Listening socket error detected by poll");
				if (_pollFds[i].revents & POLLIN)
					acceptClient();
			}
			else
			{
				if (_pollFds[i].revents & (POLLERR | POLLNVAL))
				{
					removeClient(_pollFds[i].fd);
					count--;
					continue ;
				}
				if (_pollFds[i].revents & POLLIN)
				{
					previousSize = _pollFds.size();
					handleClientRead(_pollFds[i].fd);
					if (_pollFds.size() < previousSize)
					{
						count--;
						continue ;
					}
				}
				else if (_pollFds[i].revents & POLLHUP)
				{
					removeClient(_pollFds[i].fd);
					count--;
					continue ;
				}
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
	Client	*client = NULL;

	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	try
	{
		client = new Client(clientFd);
		_clients[clientFd] = client;
		_pollFds.push_back(pfd);
	}
	catch(const std::exception &)
	{
		_clients.erase(clientFd);
		delete client;
		close(clientFd);
		return ;
	}

	if (DEBUG)
		std::cout << "Client connected: FD " << clientFd << std::endl;
}

void	Server::removeClient(int clientFd)
{
	if (DEBUG)
		std::cout << "Client disconnected: FD " << clientFd << std::endl;

	std::map<int, Client*>::iterator	itMap;
	itMap = _clients.find(clientFd);
	if (itMap != _clients.end())
	{
		delete itMap->second;
		_clients.erase(itMap);
	}

	std::vector<struct pollfd>::iterator	itVector;

	itVector = _pollFds.begin();
	while (itVector != _pollFds.end())
	{
		if (itVector->fd == clientFd)
		{
			close(itVector->fd);
			_pollFds.erase(itVector);
			break ;
		}
		itVector++;
	}
}

void	Server::handleClientRead(int clientFd)
{
	char		buffer[4096];
	ssize_t		bytesReceived;

	bytesReceived = recv(clientFd, buffer, sizeof(buffer), 0);

	if (bytesReceived > 0)
	{
		_clients[clientFd]->appendInput(buffer, bytesReceived);

		if (DEBUG)
		{
			std::cout << "[FD " << clientFd << "] Received " << bytesReceived << " bytes: ";
			std::cout.write(buffer, bytesReceived);
			std::cout << std::endl;
		}
	}
	else if (bytesReceived == 0)
		removeClient(clientFd);
	else if (bytesReceived == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
			removeClient(clientFd);
	}
}