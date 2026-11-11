/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniema3 <daniema3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 10:32:14 by daniema3          #+#    #+#             */
/*   Updated: 2026/11/11 11:00:34 by daniema3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

struct ColorTag {
	std::string	tag;
	std::string	color;
};

/* <msgcol> is not here, as it is replaced by the color tag of each level. */
static const ColorTag	COLOR_TAGS[] = {
	{"<reset>", "\033[0m"},
	{"<white>", "\033[1;37m"},
	{"<gray>", "\033[1;30m"},
	{"<red>", "\033[1;31m"},
	{"<green>", "\033[1;32m"},
	{"<yellow>", "\033[1;33m"},
	{"<cyan>", "\033[1;36m"}
};

static void	replaceAll(std::string &str, const std::string &from, const std::string &to) {
	for (size_t pos = str.find(from); pos != std::string::npos; pos = str.find(from, pos + to.size()))
		str.replace(pos, from.size(), to);
}

/* Replaces every color tag in text, <msgcol> being the color of the level. */
static std::string	applyColors(std::string text, const std::string &msgColor) {
	replaceAll(text, "<msgcol>", msgColor);
	for (size_t i = 0; i < sizeof(COLOR_TAGS) / sizeof(COLOR_TAGS[0]); i++)
		replaceAll(text, COLOR_TAGS[i].tag, COLOR_TAGS[i].color);
	return (text);
}

/*
 * Colors are applied before any argument is added, so a color tag inside an
 * argument (which may come from a client) is printed as it is.
 */
LogLine::LogLine(std::ostream &out, const std::string &level, const std::string &color,
	const std::string &text, bool enabled) : _out(out), _enabled(enabled)
{
	std::string	prefix;

	if (!_enabled)
		return ;
	prefix = color + level + "<gray>: <msgcol>";
	_text = applyColors(prefix + text + "<reset>", color);
}

/* Only the last copy prints, so the line is never printed twice. */
LogLine::LogLine(const LogLine &other) : _out(other._out), _text(other._text), _args(other._args), _enabled(other._enabled) {
	other._enabled = false;
}

LogLine::~LogLine() {
	if (_enabled)
		_out << format() << std::endl;
}

/* Replaces each {N} with its argument, leaving anything else as it is. */
std::string	LogLine::format() const {
	std::string	result;
	size_t		pos = 0;
	size_t		open;
	size_t		close;
	size_t		index;

	while ((open = _text.find('{', pos)) != std::string::npos && (close = _text.find('}', open)) != std::string::npos) {
		result += _text.substr(pos, open - pos);
		if (argIndex(_text.substr(open + 1, close - open - 1), index)) {
			result += _args[index];
			pos = close + 1;
		} else {
			result += '{';
			pos = open + 1;
		}
	}
	return (result + _text.substr(pos));
}

/* True if str is a number with a matching argument, stored in index. */
bool LogLine::argIndex(const std::string &str, size_t &index) const {
	std::istringstream in(str);
	return (!str.empty() && str.find_first_not_of("0123456789") == std::string::npos && in >> index && index < _args.size());
}
