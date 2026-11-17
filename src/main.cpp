/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 03:48:27 by daniema3          #+#    #+#             */
/*   Updated: 2026/11/17 20:23:29 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Server.hpp"
#include <exception>
#include <cstdlib>
#include <cerrno>

static bool	parsePort(const char *str, int &port)
{
	char	*end;
	long	value;
	size_t	i;

	if (!str || !*str)
		return (false);

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}

	errno = 0;
	value = std::strtol(str, &end, 10);
	if (errno == ERANGE || *end != '\0' || value < 1 || value > 65535)
		return (false);

	port = static_cast<int>(value);
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	int	port;

	if (!parsePort(argv[1], port))
	{
		std::cerr << "Error: Invalid port" << std::endl;
		return (1);
	}

	Log::info(STARTING);

	try
	{
		Server server(port, argv[2]);
		Log::ok(SERVER_LISTENING) << port;
		server.run();
	}
	catch (const std::exception &e)
	{
		Log::err(SERVER_STOPPED) << e.what();
		return (1);
	}

	return (0);
}
