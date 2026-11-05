/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:13 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/05 20:23:48 by rexposit         ###   ########.fr       */
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
		void	broadcast(const Client *excluded = NULL, const std::string &msg) const;
};


#endif
