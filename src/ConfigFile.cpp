/*
 * ConfigParser.cpp
 *
 *  Created on: 4 mar 2013
 *      Author: Cosmic
 */

#include "ConfigFile.h"

using ConfigUtil::warn;

namespace ConfigUtil {
	void warn(const std::string &error)
	{
		std::cout << "CFG - Warning: " << error << std::endl;
	}
}


ConfigFile::ConfigFile(const std::string &fn): filename(fn) {
	parse();
}

ConfigFile::~ConfigFile() {
}

void ConfigFile::remove_comment(std::string &line) const {
	if (line.find('#') != line.npos)
		line.erase(line.find('#'));
}

bool ConfigFile::only_whitespace(const std::string &line) const {
	return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigFile::valid_line(const std::string &line) const {
	// remember to run remove_comment() before this function!

	std::string tmp = line;
	tmp.erase(0, tmp.find_first_not_of("\t "));
	if (tmp[0] == '=') // no key was found
		return false;

	// look for value
	for (size_t i = tmp.find('=') + 1; i < tmp.length(); i++)
		if (tmp[i] != ' ')
			return true;

	// only whitespace was found
	return false;
}

void ConfigFile::extract_key(std::string &key, size_t const &sep_pos, const std::string &line) const {
	key = line.substr(0, sep_pos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos)
		key.erase(key.find_first_of("\t "));
}

void ConfigFile::extract_value(std::string &value, size_t const &sep_pos, const std::string &line) const {
	value = line.substr(sep_pos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extract_contents(const std::string &line) {
	// called from parse_line(), validated there

	std::string tmp = line;
	// remove leading whitespace
	tmp.erase(0, tmp.find_first_not_of("\t "));
	size_t sep_pos = tmp.find('=');

	std::string key, value;
	extract_key(key, sep_pos, tmp);
	extract_value(value, sep_pos, tmp);
	if (!key_exists(key))
		contents.insert(std::pair<std::string, std::string>(key, value));
	else
		warn(key + " was declared at least twice in configuration file.");
}

void ConfigFile::parse_line(const std::string &line, size_t const line_number) {
	// validate line
	if (line.find('=') == line.npos) {
		warn("Cannot find separator on line: " + Convert::T_to_string(line_number) + "\n");
		return;
	}
	if (!valid_line(line)) {
		warn("Bad format on line: " + Convert::T_to_string(line_number) + "\n");
		return;
	}

	extract_contents(line);
}

bool ConfigFile::key_exists(const std::string &key) const {
	return contents.find(key) != contents.end();
}

void ConfigFile::parse() {
	std::ifstream file;
	file.open(filename.c_str());
	if (!file) {
		warn("File " + filename + " could not be found.");
		return;
	}

	std::string line;
	size_t line_number = 0;
	while (std::getline(file, line))
	{
		line_number++;
		std::string tmp = line;
		if (tmp.empty() || tmp == "\r" || tmp == "\n")
			continue;

		remove_comment(tmp);
		if (only_whitespace(tmp))
			continue;

		parse_line(tmp, line_number);
	}

	file.close();
}

