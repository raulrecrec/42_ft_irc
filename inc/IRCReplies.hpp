/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplies.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 15:44:09 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/18 15:00:31 by rexposit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLIES_HPP
# define IRCREPLIES_HPP

#include <string>

const std::string	RPL_WELCOME = ":Welcome to the rexposit & daniema3 IRC Network!\r\n";

const std::string	ERR_PASSWDMISMATCH = ":ircserv 464 * :Password incorrect\r\n";
const std::string	ERR_NEEDMOREPARAMS = ":ircserv 461 * PASS :Not enough parameters\r\n";
const std::string	ERR_NONICKNAMEGIVEN = ":ircserv 431 * :No nickname given\r\n";
const std::string	ERR_NICKNAMEINUSE = ":Nickname is already in use\r\n";
const std::string	ERR_ERRONEUSNICKNAME = ":Erroneous nickname\r\n";
const std::string	ERR_USER_NEEDMOREPARAMS = ":ircserv 461 * USER :Not enough parameters\r\n";
const std::string	ERR_ALREADYREGISTRED = ":You may not reregister\r\n";
const std::string	ERR_NOORIGIN = ":ircserv 409 * :No origin specified\r\n";
const std::string	ERR_UNKNOWNCOMMAND = ":Unknown command\r\n";

#endif