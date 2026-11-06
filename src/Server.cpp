/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:25:04 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/06 15:37:00 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>

Server::Server(int port, const std::string &password)
	: _listenFd(-1), _port(port), _password(password)
{
	initSocket();
}

Server::~Server()
{
	if (_listenFd != -1)
		close(_listenFd);
}

void Server::initSocket()
{
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);

	if (_listenFd == -1)
		throw std::runtime_error("Failed to create server socket");
}