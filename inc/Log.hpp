#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

#define COL_RESET	"\033[0m"
#define COL_GRAY	"\033[1;30m"
#define COL_RED		"\033[1;31m"
#define COL_GREEN	"\033[1;32m"
#define COL_YELLOW	"\033[1;33m"
#define COL_CYAN	"\033[1;36m"

#define TAG_INFO	"\033[0;36mINFO" COL_GRAY ": "
#define TAG_OK		"\033[0;32mOK" COL_GRAY ": "
#define TAG_WARN	"\033[0;33mWARNING" COL_GRAY ": "
#define TAG_ERR		"\033[0;31mERROR" COL_GRAY ": "

struct LogInfo	{ const std::string text; };
struct LogOk	{ const std::string text; };
struct LogWarn	{ const std::string text; };
struct LogErr	{ const std::string text; };

/*
 - Messages
 */

const LogInfo	STARTING = {"Starting IRC server"};

/*
 * Prints the messages above with the prefix and colors of their level. The
 * optional detail, of any printable type, is shown after the message:
 *
 *   Log::err(ERR_USAGE);                   Error: Expected usage: ...
 *   Log::info(INFO_CHANNEL_CREATED, name); Info: Channel created - #42
 *
 * info and ok go to the standard output, warn and err to the error output.
 */
class Log {

	public:

		/* INFO */

		static void	info(const LogInfo &msg) {
			print(std::cout, TAG_INFO, COL_CYAN, msg.text);
		}

		template <typename T> static void	info(const LogInfo &msg, const T &detail) {
			print(std::cout, TAG_INFO, COL_CYAN, msg.text, detail);
		}

		/* OK */

		static void	ok(const LogOk &msg) {
			print(std::cout, TAG_OK, COL_GREEN, msg.text);
		}

		template <typename T> static void ok(const LogOk &msg, const T &detail) {
			print(std::cout, TAG_OK, COL_GREEN, msg.text, detail);
		}

		/* WARN */

		static void	warn(const LogWarn &msg) {
			print(std::cerr, TAG_WARN, COL_YELLOW, msg.text);
		}

		template <typename T> static void warn(const LogWarn &msg, const T &detail) {
			print(std::cerr, TAG_WARN, COL_YELLOW, msg.text, detail);
		}

		/* ERR */

		static void	err(const LogErr &msg) {
			print(std::cerr, TAG_ERR, COL_RED, msg.text);
		}

		template <typename T> static void 	err(const LogErr &msg, const T &detail) {
			print(std::cerr, TAG_ERR, COL_RED, msg.text, detail);
		}

	private:

		Log();

		static void	print(std::ostream &out, const char *tag, const char *color, const std::string text) {
			out << tag << color << text << COL_RESET << std::endl;
		}

		template <typename T> static void print(std::ostream &out, const char *tag, const char *color, const char *text, const T &detail) {
			out << tag << color << text << COL_GRAY " - " << color << detail << COL_RESET << std::endl;
		}
};

#endif
