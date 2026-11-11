/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniema3 <daniema3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 03:48:27 by daniema3          #+#    #+#             */
/*   Updated: 2026/11/09 05:24:14 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "Server.hpp"
#include <iostream>
#include <exception>

int main()
{
	Log::info(STARTING);
	try
	{
		Server server(6667, "password123");

		std::cout << "Server listening on port 6667" << std::endl;

		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
