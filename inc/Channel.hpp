/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rexposit <rexposit@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:17:13 by rexposit          #+#    #+#             */
/*   Updated: 2026/11/16 06:55:58 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <cstddef>
#include <set>
#include <string>
#include <vector>

class Client;

/* Why a client can or cannot join a channel. */
enum JoinStatus {
	JOIN_OK,
	JOIN_INVITE_ONLY,
	JOIN_BAD_KEY,
	JOIN_FULL,
};

/*
 * One channel and everything about it: who is in, who runs it, its topic and
 * its modes. It only keeps the state; the commands decide what to do with it
 * and which replies to send.
 *
 * It keeps pointers to its clients, so a client that disconnects must be
 * removed from every channel before it is deleted.
 */
class Channel {
	private:
		std::string				_name;
		std::string				_topic;
		std::vector<Client*>	_members;
		std::set<const Client*>	_operators;
		std::set<const Client*>	_invited;
		bool					_inviteOnly;
		bool					_topicRestricted;
		std::string				_key;
		size_t					_limit;

	public:
		Channel(const std::string &name);
		~Channel();

		const std::string	&getName() const;

		/* - Members */

		/*
		 * Adds the client to the channel. The first one to join becomes its
		 * operator, and joining uses up the client's invitation, if any.
		 */
		void		addClient(Client &client);

		/* Takes the client out of the channel, its operators and its invites. */
		void		removeClient(Client &client);

		bool		hasClient(const Client &client) const;
		bool		isEmpty() const;

		/*
		 * The members' nicknames separated by spaces, in the order they
		 * joined, with '@' before the operators. It is what the names list
		 * shows when someone joins.
		 */
		std::string	getNamesList() const;

		/*
		 * Queues the line for every member, except the excluded one (usually
		 * whoever caused it, who already knows).
		 */
		void		broadcast(const std::string &line, const Client *excluded = NULL) const;

		/* - Operators */

		bool		isOperator(const Client &client) const;
		void		setOperator(Client &client, bool isOperator);

		/* - Topic (TOPIC, +t) */

		const std::string	&getTopic() const;
		void				setTopic(const std::string &topic);
		bool				isTopicRestricted() const;
		void				setTopicRestricted(bool restricted);

		/* - Invites (INVITE, +i) */

		bool		isInviteOnly() const;
		void		setInviteOnly(bool inviteOnly);
		void		invite(Client &client);
		bool		isInvited(const Client &client) const;

		/* - Key (+k) and user limit (+l). Empty key and limit 0 mean none */

		const std::string	&getKey() const;
		void				setKey(const std::string &key);
		size_t				getLimit() const;
		void				setLimit(size_t limit);

		/*
		 * Whether the client may join with that key, checking the invite,
		 * the key and the limit.
		 */
		JoinStatus	canJoin(const Client &client, const std::string &key) const;

		/*
		 * The active modes as MODE shows them, followed by their values:
		 * "+itkl secret 10".
		 */
		std::string	getModes() const;
};

#endif
