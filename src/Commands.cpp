/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:28:27 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/18 15:04:51 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "IRCReplies.hpp"
#include "Log.hpp"

void	Commands::execute(Server &server, Client &client, const Message &msg)
{
	if (msg.command == "PING")
		ping(server, client, msg);
	else if (msg.command == "CAP")
		cap(server, client, msg);
	else if (msg.command == "PASS")
		pass(server, client, msg);
	else if (msg.command == "NICK")
		nick(server, client, msg);
	else if (msg.command == "USER")
		user(server, client, msg);
	else if (!msg.command.empty())
	{
		std::string	nickname;

		nickname = client.getNick();
		if (nickname.empty())
			nickname = "*";
		
		server.sendToClient(client.getFd(), ":ircserv 421 " + nickname + " " + msg.command + " " + ERR_UNKNOWNCOMMAND);
	}
}

void	Commands::ping(Server &server, Client &client, const Message &msg)
{
	if (msg.params.empty())
	{
		server.sendToClient(client.getFd(), ERR_NOORIGIN);
		return ;
	}

	server.sendToClient(client.getFd(), "PONG :" + msg.params[0] + "\r\n");
}

void	Commands::cap(Server &server, Client &client, const Message &msg)
{
	if (msg.params.empty())
		return ;

	if (msg.params[0] == "LS")
	{
		client.setCapNegotiating(true);
		server.sendToClient(client.getFd(), ":ircserv CAP * LS :\r\n");
	}
	else if (msg.params[0] == "END")
	{
		client.setCapNegotiating(false);
		server.tryRegister(&client);
	}
}

void	Commands::pass(Server &server, Client &client, const Message &msg)
{
	if (client.isRegistered())
		server.sendToClient(client.getFd(), ":ircserv 462 " + client.getNick() + " " + ERR_ALREADYREGISTRED);
	else if (msg.params.empty())
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS);
	else if (server.checkPassword(msg.params[0]))
	{
		client.setAuthorized(true);
		Log::debug(CLIENT_AUTHORIZED) << client.getFd();
		server.tryRegister(&client);
	}
	else
		server.sendToClient(client.getFd(), ERR_PASSWDMISMATCH);
}

void	Commands::nick(Server &server, Client &client, const Message &msg)
{
	if (msg.params.empty() || msg.params[0].empty())
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN);
	else
	{
		if (!server.isValidNick(msg.params[0]))
			server.sendToClient(client.getFd(), ":ircserv 432 * " + msg.params[0] + " " + ERR_ERRONEUSNICKNAME);
		else
		{
			Client	*existingClient;

			existingClient = server.findClient(msg.params[0]);
			if (existingClient == NULL || existingClient == &client)
			{
				client.setNick(msg.params[0]);
				Log::debug(CLIENT_NICK_SET) << client.getFd() << client.getNick();
				server.tryRegister(&client);
			}
			else
				server.sendToClient(client.getFd(), ":ircserv 433 * " + msg.params[0] + " " + ERR_NICKNAMEINUSE);
		}
	}
}

void	Commands::user(Server &server, Client &client, const Message &msg)
{
	if (client.isRegistered())
		server.sendToClient(client.getFd(), ":ircserv 462 " + client.getNick() + " " + ERR_ALREADYREGISTRED);
	else if (msg.params.size() < 4)
		server.sendToClient(client.getFd(), ERR_USER_NEEDMOREPARAMS);
	else if (msg.params[3].find_first_not_of(" \t") == std::string::npos)
		server.sendToClient(client.getFd(), ERR_USER_NEEDMOREPARAMS);
	else
	{
		client.setUsername(msg.params[0]);
		server.tryRegister(&client);
	}
}
