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
		void	broadcast(const Client &excluded, const std::string &msg) const;
};


#endif
