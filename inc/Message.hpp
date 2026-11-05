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
