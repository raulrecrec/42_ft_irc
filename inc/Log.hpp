#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/* Enabled by building with `make debug`. */
#ifdef IRC_DEBUG
	const bool	DEBUG = true;
#else
	const bool	DEBUG = false;
#endif

struct LogDebug	{ const std::string text; };
struct LogInfo	{ const std::string text; };
struct LogOk	{ const std::string text; };
struct LogWarn	{ const std::string text; };
struct LogErr	{ const std::string text; };

/*
 - Messages
 *
 * {0}, {1}... are replaced by the arguments given with <<, in that order, and
 * can be repeated or reordered. <white>, <gray>, <red>, <green>, <yellow>,
 * <cyan> and <reset> change the color, <msgcol> goes back to the color of
 * the level.
 */

const LogDebug	TEST = {"Testing debug output"};
const LogDebug	CLIENT_CONNECTED = {"Client <white>{0} <msgcol>connected"};
const LogDebug	CLIENT_DISCONNECTED = {"Client <white>{0} <msgcol>disconnected"};
const LogDebug	DATA_RECEIVED = {"Received <white>{1} <msgcol>bytes from client <white>{0}"};
const LogDebug	COMMAND_RECEIVED = {"Client <white>{0} <msgcol>sent: <white>{1}"};

const LogInfo	STARTING = {"Starting IRC server"};

const LogOk		SERVER_LISTENING = {"Server listening on port <white>{0}"};

const LogErr	SERVER_STOPPED = {"Server stopped: <white>{0}"};
const LogErr	SERVER_SOCKET_CREATE_FAILED = {"Failed to create server socket"};
const LogErr	SERVER_SOCKET_NONBLOCK_FAILED = {"Failed to set server socket to non-blocking mode"};
const LogErr	SERVER_SOCKET_BIND_FAILED = {"Failed to bind server socket"};
const LogErr	SERVER_SOCKET_LISTEN_FAILED = {"Failed to listen on server socket"};
const LogErr	SERVER_POLL_FAILED = {"Failed to poll server sockets"};
const LogErr	LISTEN_SOCKET_POLL_ERROR = {"Listening socket error detected by poll"};
const LogErr	CLIENT_ACCEPT_FAILED = {"Failed to accept client connection"};

/*
 * One log line being built. It collects the arguments given with << and
 * prints the message with them when the statement that created it ends.
 * Placeholders and color tags that do not exist are printed as they are.
 */
class LogLine {

	public:

		LogLine(std::ostream &out, const std::string &level, const std::string &color, const std::string &text, bool enabled);
		LogLine(const LogLine &other);
		~LogLine();

		template <typename T> LogLine	&operator<<(const T &arg) {
			if (_enabled) {
				std::ostringstream	str;
				str << arg;
				_args.push_back(str.str());
			}
			return (*this);
		}

	private:

		LogLine	&operator=(const LogLine &other);

		std::string	format() const;
		bool		argIndex(const std::string &str, size_t &index) const;

		std::ostream				&_out;
		std::string					_text;
		std::vector<std::string>	_args;
		mutable bool				_enabled;
};

/*
 * Prints the messages above with the prefix and color of their level, so
 * each level only accepts its own messages:
 *
 *   Log::info(STARTING);                         INFO: Starting IRC server
 *   Log::ok(SERVER_LISTENING) << 6667;           OK: Server listening on port 6667
 *   Log::debug(COMMAND_RECEIVED) << 5 << line;   DEBUG: Client 5 sent: NICK a
 *
 * debug only prints when DEBUG is enabled. debug, info and ok go to the
 * standard output, warn and err to the error output.
 */
class Log {

	public:
		// Sends a LogDebug message at the DEBUG level to std::cout, if debug is enabled
		static LogLine debug(const LogDebug &msg) {
			return (LogLine(std::cout, "DEBUG", "<white>", msg.text, DEBUG));
		}

		// Sends a LogInfo message at the INFO level to std::cout
		static LogLine info(const LogInfo &msg) {
			return (LogLine(std::cout, "INFO", "<cyan>", msg.text, true));
		}

		// Sends a LogOk message at the OK level to std::cout
		static LogLine ok(const LogOk &msg) {
			return (LogLine(std::cout, "OK", "<green>", msg.text, true));
		}

		// Sends a LogWarn message at the WARN level to std::cerr
		static LogLine warn(const LogWarn &msg) {
			return (LogLine(std::cerr, "WARNING", "<yellow>", msg.text, true));
		}

		// Sends a LogErr message at the ERROR level to std::cerr
		static LogLine err(const LogErr &msg) {
			return (LogLine(std::cerr, "ERROR", "<red>", msg.text, true));
		}
	private:
		Log();
};

#endif
