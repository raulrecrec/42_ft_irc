/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 15:25:04 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/12 19:38:07 by rexposit         ###   ########.fr       */
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
#include "Log.hpp"
#include <cctype>

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
		throw std::runtime_error(SERVER_SOCKET_CREATE_FAILED.text);

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
		throw std::runtime_error(SERVER_SOCKET_NONBLOCK_FAILED.text);
	}

	result = bind(_listenFd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr));

	if (result == -1)
	{
		closeSocket();
		throw std::runtime_error(SERVER_SOCKET_BIND_FAILED.text);
	}

	result = listen(_listenFd, SOMAXCONN);

	if (result == -1)
	{
		closeSocket();
		throw std::runtime_error(SERVER_SOCKET_LISTEN_FAILED.text);
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
			throw std::runtime_error(SERVER_POLL_FAILED.text);
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
					throw std::runtime_error(LISTEN_SOCKET_POLL_ERROR.text);
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
				if (_pollFds[i].revents & POLLOUT)
				{
					previousSize = _pollFds.size();
					handleClientWrite(_pollFds[i]);
					if (_pollFds.size() < previousSize)
					{
						count--;
						continue ;
					}
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

		throw std::runtime_error(CLIENT_ACCEPT_FAILED.text);
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

	Log::debug(CLIENT_CONNECTED) << clientFd;
}

void	Server::removeClient(int clientFd)
{
	Log::debug(CLIENT_DISCONNECTED) << clientFd;

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
	Client		*client;
	char		buffer[4096];
	ssize_t		bytesReceived;
	std::string	line;
	LineStatus	status;

	client = _clients[clientFd];
	bytesReceived = recv(clientFd, buffer, sizeof(buffer), 0);

	if (bytesReceived == 0)
		removeClient(clientFd);
	else if (bytesReceived == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
			removeClient(clientFd);
	}
	else if (bytesReceived > 0)
	{
		Log::debug(DATA_RECEIVED) << clientFd << bytesReceived;
		client->appendInput(buffer, bytesReceived);

		status = LINE_READY;
		while (status == LINE_READY)
		{
			status = client->extractLine(line);
			switch (status)
			{
			case LINE_READY:
			{
				if (line == "PASS" || line.compare(0, 5, "PASS ") == 0)
					Log::debug(COMMAND_RECEIVED) << clientFd << "PASS [REDACTED]";
				else
					Log::debug(COMMAND_RECEIVED) << clientFd << line;

				if (line.compare(0, 5, "PING ") == 0)
					sendToClient(clientFd, "PONG" + line.substr(4) + "\r\n");

				if (line.compare(0, 6, "RELAY ") == 0)
				{
					std::map<int, Client*>::iterator itMap;

					itMap = _clients.begin();
					while (itMap != _clients.end())
					{
						if (itMap->first != clientFd)
							sendToClient(itMap->first, ":ircserv NOTICE * :[RELAY] " + line.substr(6) + "\r\n");
						itMap++;
					}
				}

				if (line == "PASS" || line.compare(0, 5, "PASS ") == 0)
				{
					if (line.size() <= 5)
						sendToClient(clientFd, ERR_NEEDMOREPARAMS.text);
					else if (_password == line.substr(5))
					{
						client->setAuthorized(true);
						Log::debug(CLIENT_AUTHORIZED) << clientFd;
					}
					else
						sendToClient(clientFd, ERR_PASSWDMISMATCH.text);
				}

				if (line == "NICK" || line.compare(0, 5, "NICK ") == 0)
				{
					if (line.size() <= 5)
						sendToClient(clientFd, ERR_NONICKNAMEGIVEN.text);
					else
					{
						std::string	nickname;
						Client		*existingClient;

						nickname = line.substr(5);
						if (!isValidNick(nickname))
							sendToClient(clientFd, ":ircserv 432 * " + nickname + " " + ERR_ERRONEUSNICKNAME.text);
						else
						{
							existingClient = findClient(nickname);
							if (existingClient == NULL || existingClient == client)
							{
								client->setNick(nickname);
								Log::debug(CLIENT_NICK_SET) << clientFd << nickname;
							}
							else
								sendToClient(clientFd, ":ircserv 433 * " + nickname + " " + ERR_NICKNAMEINUSE.text);
						}
					}
				}
				break ;
			}
			case LINE_INCOMPLETE:
				return ;
			case LINE_TOO_LONG:
				removeClient(clientFd);
				return ;
			}
		}
	}
}

void	Server::handleClientWrite(struct pollfd &pfd)
{
	Client	*client;
	int		clientFd;
	ssize_t	bytesSent;

	clientFd = pfd.fd;
	client = _clients[clientFd];
	if (client->getOutputBuffer().empty())
	{
		pfd.events &= ~POLLOUT;
		return ;
	}

	bytesSent = send(clientFd, client->getOutputBuffer().c_str(), client->getOutputBuffer().size(), MSG_NOSIGNAL);

	if (bytesSent == 0)
	{
		removeClient(clientFd);
		return ;
	}
	else if (bytesSent == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
			removeClient(clientFd);
		return ;
	}
	else if (bytesSent > 0)
	{
		client->consumeOutput(bytesSent);
		if (client->getOutputBuffer().empty())
			pfd.events &= ~POLLOUT;
	}
}

bool	Server::isValidNick(const std::string &nickname)
{
	if (nickname.empty() || nickname.size() > 9)
		return (false);

	const std::string	special = "[]\\^_`{|}";

	if (!std::isalpha(static_cast<unsigned char>(nickname[0])) && special.find(nickname[0]) == std::string::npos)
		return (false);

	size_t	i;

	i = 1;
	while (i < nickname.size())
	{
		if (!std::isalnum(static_cast<unsigned char>(nickname[i])) && special.find(nickname[i]) == std::string::npos && nickname[i] != '-')
			return (false);
		i++;
	}

	return (true);
}

std::string	Server::normalizeNick(const std::string &nickname) const
{
	std::string	nicknameCpy;
	size_t		i;

	nicknameCpy = nickname;
	i = 0;
	while (i < nickname.size())
	{
		if (nicknameCpy[i] == '{')
			nicknameCpy[i] = '[';
		else if (nicknameCpy[i] == '}')
			nicknameCpy[i] = ']';
		else if (nicknameCpy[i] == '|')
			nicknameCpy[i] = '\\';
		else if (nicknameCpy[i] == '~')
			nicknameCpy[i] = '^';
		else
			nicknameCpy[i] = std::tolower(static_cast<unsigned char>(nicknameCpy[i]));
		i++;
	}

	return (nicknameCpy);
}

Client	*Server::findClient(const std::string &nick)
{
	std::map<int, Client*>::iterator	itMap;
	std::string							nickNormalized;
	std::string							clientNormalized;

	if (nick.empty())
		return (NULL);

	nickNormalized = normalizeNick(nick);
	itMap = _clients.begin();
	while (itMap != _clients.end())
	{
		clientNormalized = normalizeNick(itMap->second->getNick());
		if (clientNormalized == nickNormalized)
			return (itMap->second);
		itMap++;
	}

	return (NULL);
}

void	Server::sendToClient(int clientFd, const std::string &msg)
{
	std::vector<struct pollfd>::iterator	itVector;

	if (msg.empty())
		return ;

	itVector = _pollFds.begin();
	while (itVector != _pollFds.end())
	{
		if (itVector->fd == clientFd)
		{
			_clients[clientFd]->appendOutput(msg.c_str(), msg.size());
			itVector->events |= POLLOUT;
			break ;
		}
		itVector++;
	}
}