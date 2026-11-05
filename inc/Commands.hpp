/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:22 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/05 20:17:22 by rexposit         ###   ########.fr       */
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
	public:
		static void execute(const Server &server, const Client &client, const Message &msg);
};

#endif
