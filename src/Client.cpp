/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 19:37:01 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/11 13:17:14 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd)
{
}

Client::~Client()
{
}

int	Client::getFd() const
{
	return (_fd);
}

const std::string	&Client::getOutputBuffer() const
{
	return (_outputBuffer);
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