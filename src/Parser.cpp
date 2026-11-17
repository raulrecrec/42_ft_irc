/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 16:18:08 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/17 17:26:14 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Message.hpp"
#include <sstream>
#include <cctype>

Message	Parser::parseMessage(const std::string &line)
{
	Message				msg;
	std::istringstream	stream(line);
	std::string			param;
	std::string			rest;
	size_t				i;

	stream >> msg.command;
	i = 0;
	while (i < msg.command.size())
	{
		msg.command[i] = std::toupper(static_cast<unsigned char>(msg.command[i]));
		i++;
	}

	while (stream >> param)
	{
		if ((!param.empty() && param[0] == ':') || msg.params.size() == 14)
		{
			if (param[0] == ':')
				param.erase(0, 1);
			std::getline(stream, rest);
			param += rest;
			msg.params.push_back(param);
			break ;
		}
		else
			msg.params.push_back(param);
	}

	return (msg);
}
