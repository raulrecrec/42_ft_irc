/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:25 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/05 20:17:25 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

/* One IRC command received from a client, already split into its parts. */
struct Message
{
	// The command name, always in upper case ("JOIN", "PRIVMSG"...).
	std::string					command;

	// The command's arguments, in order. The last one may contain spaces
	std::vector<std::string>	params;
};

#endif
