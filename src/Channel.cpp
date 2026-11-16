/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniema3 <daniema3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 06:27:11 by daniema3          #+#    #+#             */
/*   Updated: 2026/11/16 06:56:46 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>
#include <sstream>

Channel::Channel(const std::string &name) : _name(name), _inviteOnly(false), _topicRestricted(false), _limit(0) {}

Channel::~Channel() {}

const std::string	&Channel::getName() const {
	return (_name);
}

/*
 - Members
 */

void	Channel::addClient(Client &client) {
	if (hasClient(client))
		return ;
	if (_members.empty())
		_operators.insert(&client);
	_members.push_back(&client);
	_invited.erase(&client);
}

void	Channel::removeClient(Client &client) {
	std::vector<Client*>::iterator	it;

	it = std::find(_members.begin(), _members.end(), &client);
	if (it != _members.end())
		_members.erase(it);
	_operators.erase(&client);
	_invited.erase(&client);
}

bool	Channel::hasClient(const Client &client) const {
	return (std::find(_members.begin(), _members.end(), &client) != _members.end());
}

bool	Channel::isEmpty() const {
	return (_members.empty());
}

std::string	Channel::getNamesList() const {
	std::string	names;

	for (size_t i = 0; i < _members.size(); i++) {
		if (i > 0)
			names += " ";
		if (isOperator(*_members[i]))
			names += "@";
		names += _members[i]->getNick();
	}
	return (names);
}

void	Channel::broadcast(const std::string &line, const Client *excluded) const {
	for (size_t i = 0; i < _members.size(); i++) {
		if (_members[i] != excluded)
			_members[i]->queue(line);
	}
}

/*
 - Operators
 */

bool	Channel::isOperator(const Client &client) const {
	return (_operators.count(&client) > 0);
}

void	Channel::setOperator(Client &client, bool isOperator) {
	if (isOperator)
		_operators.insert(&client);
	else
		_operators.erase(&client);
}

/*
 - Topic
 */

const std::string	&Channel::getTopic() const {
	return (_topic);
}

void	Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

bool	Channel::isTopicRestricted() const {
	return (_topicRestricted);
}

void	Channel::setTopicRestricted(bool restricted) {
	_topicRestricted = restricted;
}

/*
 - Invites
 */

bool	Channel::isInviteOnly() const {
	return (_inviteOnly);
}

void	Channel::setInviteOnly(bool inviteOnly) {
	_inviteOnly = inviteOnly;
}

void	Channel::invite(Client &client) {
	_invited.insert(&client);
}

bool	Channel::isInvited(const Client &client) const {
	return (_invited.count(&client) > 0);
}

/*
 - Key and limit
 */

const std::string	&Channel::getKey() const {
	return (_key);
}

void	Channel::setKey(const std::string &key) {
	_key = key;
}

size_t	Channel::getLimit() const {
	return (_limit);
}

void	Channel::setLimit(size_t limit) {
	_limit = limit;
}

/* An invitation only lets the client past +i, not past the key or the limit. */
JoinStatus	Channel::canJoin(const Client &client, const std::string &key) const {
	if (_inviteOnly && !isInvited(client))
		return (JOIN_INVITE_ONLY);
	if (!_key.empty() && key != _key)
		return (JOIN_BAD_KEY);
	if (_limit > 0 && _members.size() >= _limit)
		return (JOIN_FULL);
	return (JOIN_OK);
}

std::string	Channel::getModes() const {
	std::string			modes;
	std::ostringstream	values;

	modes = "+";
	if (_inviteOnly)
		modes += "i";
	if (_topicRestricted)
		modes += "t";
	if (!_key.empty()) {
		modes += "k";
		values << " " << _key;
	}
	if (_limit > 0) {
		modes += "l";
		values << " " << _limit;
	}
	return (modes + values.str());
}
