/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:13 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/09 04:46:39 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>

class Client;

class Channel
{
	public:
		bool	addClient(const Client &client);
		bool	removeClient(const Client &client);
		bool	hasClient(const Client &client) const;
		bool	isOperator(const Client &client) const;
		void	broadcast(const std::string &msg, const Client *excluded = NULL) const;
};

#endif
