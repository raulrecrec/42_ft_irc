#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>

class Channel;
class Client;

/*
 * The server: owns every connected client and the list of channels.
 *
 * The server side owns this class. Only what the channel side relies on is
 * listed here; the server side adds whatever else it needs.
 */
class Server
{
	private:
		/*
		 * Every existing channel, by name. Lives in the server so commands
		 * can reach it, but only the channel side creates, changes and
		 * deletes channels.
		 */
		std::map<std::string, Channel*>	channels;

	public:
		/*
		 * Finds the connected client using that nickname, ignoring case
		 * the way IRC does. NULL when nobody has it.
		 * Used to deliver private messages, kick or invite someone...
		 */
		Client*	findClient(const std::string &nick);

		Channel* findChannel(const std::string &name);

		void sendToClient(const Client &client, const std::string &msg);
};

#endif
