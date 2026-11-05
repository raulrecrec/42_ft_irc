#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

class Message;

class Parser
{
	public:
		static Message parseMessage(const std::string &msg);
};

#endif
