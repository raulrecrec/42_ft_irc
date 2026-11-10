/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:16 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/10 15:00:09 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

/* One user connected to the server. */
class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_inputBuffer;
		std::string	_outputBuffer;

	public:
		Client(int fd);
		~Client();

		int	getFd() const;

		/*
		 * The client's current nickname, empty until it has set one.
		 * Used to find users and to address replies to them.
		 */
		const std::string&	getNick() const;

		const std::string&	getUsername() const;

		/*
		 * Who this client is, as other users see it ("nick!user@host").
		 * Goes at the start of every message this client causes, so that
		 * the receivers know who sent it.
		 */
		std::string			prefix() const;

		/*
		 * Leaves a line ready to be sent to this client. Nothing is sent
		 * right away: the server side delivers it when the connection is
		 * ready. Commands use this for every reply and every message they
		 * forward.
		 */
		void				queue(const std::string &line);

		bool				isAuthorized() const;
		bool				isRegistered() const;

		void				appendInput(const std::string &data);
		void				appendOutput(const std::string &data);
};

#endif
