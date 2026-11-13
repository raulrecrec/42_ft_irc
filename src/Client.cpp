/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 19:37:01 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/13 19:21:46 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd), _authorized(false), _registered(false), _capNegotiating(false)
{
}

Client::~Client()
{
}

int	Client::getFd() const
{
	return (_fd);
}

const std::string &Client::getNick() const
{
	return (_nickname);
}

const std::string&	Client::getUsername() const
{
	return (_username);
}

const std::string	&Client::getOutputBuffer() const
{
	return (_outputBuffer);
}

void	Client::setAuthorized(bool authorized)
{
	_authorized = authorized;
}

void	Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void	Client::setUsername(const std::string &username)
{
	_username = username;
}

void	Client::setRegistered(bool registered)
{
	_registered = registered;
}

void	Client::setCapNegotiating(bool negotiating)
{
	_capNegotiating = negotiating;
}



bool	Client::isAuthorized() const
{
	return(_authorized);
}

bool	Client::isRegistered() const
{
	return (_registered);
}

bool Client::isCapNegotiating() const
{
	return (_capNegotiating);
}

void	Client::appendInput(const char *data, size_t len)
{
	_inputBuffer.append(data, len);
}

void	Client::appendOutput(const char *data, size_t len)
{
	_outputBuffer.append(data, len);
}

LineStatus	Client::extractLine(std::string &line)
{
	size_t	pos;

	pos = _inputBuffer.find("\r\n");
	if (pos == std::string::npos)
	{
		if (_inputBuffer.size() > 511 || (_inputBuffer.size() == 511 && _inputBuffer[510] != '\r'))
			return (LINE_TOO_LONG);
		return (LINE_INCOMPLETE);
	}
	if (pos > 510)
		return (LINE_TOO_LONG);

	line = _inputBuffer.substr(0, pos);
	_inputBuffer.erase(0, pos + 2);
	return (LINE_READY);
}

void	Client::consumeOutput(size_t bytesSent)
{
	_outputBuffer.erase(0, bytesSent);
}