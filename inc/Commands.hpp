#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>

class Client;
class Server;
struct Message;

class Commands
{
	public:
		static void execute(const Server &server, const Client &client, const Message &msg);
};

#endif
