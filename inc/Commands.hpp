/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:22 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/17 19:03:45 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>

class Client;
class Server;
struct Message;

class Commands
{
	private:
		static void	ping(Server &server, Client &client, const Message &msg);
		static void	cap(Server &server, Client &client, const Message &msg);
		static void	pass(Server &server, Client &client, const Message &msg);
		static void	nick(Server &server, Client &client, const Message &msg);
		static void	user(Server &server, Client &client, const Message &msg);

	public:
		static void	execute(Server &server, Client &client, const Message &msg);
};

#endif
