/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniema3 <daniema3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 03:48:27 by daniema3          #+#    #+#             */
/*   Updated: 2026/11/11 10:35:39 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Server.hpp"
#include <exception>

int main()
{
	Log::debug(TEST);
	Log::info(STARTING);
	try
	{
		Server server(6667, "password123");
		Log::ok(SERVER_LISTENING) << 6667;
		server.run();
	}
	catch (const std::exception &e)
	{
		Log::err(SERVER_STOPPED) << e.what();
		return (1);
	}
	return (0);
}
