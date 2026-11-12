/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:28 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/12 19:31:11 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Channel;
class Client;

/*
 * The server: owns every connected client and the list of channels.
 *
 * The server side owns this class. Only what the channel side relies on is
 * listed here; the server side adds whatever else it needs.
 */
class Server
{
	private:
		int			_listenFd;
		int			_port;
		std::string	_password;

		std::vector<struct pollfd>	_pollFds;
		std::map<int, Client*>		_clients;

		void	initSocket();
		void	closeSocket();

		void		acceptClient();
		void		removeClient(int clientFd);
		void		handleClientRead(int clientFd);
		void		handleClientWrite(struct pollfd &pfd);
		bool		isValidNick(const std::string &nickname);
		std::string	normalizeNick(const std::string &nickname) const;
		/*
		 * Every existing channel, by name. Lives in the server so commands
		 * can reach it, but only the channel side creates, changes and
		 * deletes channels.
		 */
		std::map<std::string, Channel*>	channels;

	public:
		Server(int port, const std::string &password);
		~Server();

		void	run();
		

		/*
		 * Finds the connected client using that nickname, ignoring case
		 * the way IRC does. NULL when nobody has it.
		 * Used to deliver private messages, kick or invite someone...
		 */
		Client*		findClient(const std::string &nick);
		Channel*	findChannel(const std::string &name);

		void		sendToClient(int clientFd, const std::string &msg);
};

#endif
