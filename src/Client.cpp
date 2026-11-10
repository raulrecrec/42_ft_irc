/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 19:37:01 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/10 19:47:45 by rexposit         ###   ########.fr       */
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

void	Client::appendInput(const char *data, size_t len)
{
	_inputBuffer.append(data, len);
}