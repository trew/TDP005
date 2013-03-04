/*
 * ConfigParser.h
 *
 *  Created on: 4 mar 2013
 *      Author: Cosmic
 */

#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stdexcept>

namespace ConfigUtil {
	void warn(const std::string &error);

}

/**
 * http://www.dreamincode.net/forums/topic/183191-create-a-simple-configuration-file-parser/
 */
class Convert {
public:
	class parse_exception: public std::runtime_error {
	public:
		parse_exception(const std::string &w) : std::runtime_error(w) {}
	};

	template<typename T>
	static std::string T_to_string(T const &val) {
		std::ostringstream os;
		os << val;
		return os.str();
	}

	template<typename T>
	static T string_to_T(const std::string &val) {
		std::istringstream istr(val);
		T returnVal;
		if (!(istr >> returnVal))
			throw parse_exception("CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n");
		return returnVal;
	}
};

class ConfigFile {
public:
	ConfigFile(const std::string &filename);
	virtual ~ConfigFile();

	bool key_exists(const std::string &key) const;

	template <typename V> V get_value(const std::string &key, V const &default_value = V()) const {
		if (!key_exists(key))
			return default_value;
		try {
			return Convert::string_to_T<V>(contents.find(key)->second);
		} catch (Convert::parse_exception &e) {
			ConfigUtil::warn(std::string(e.what()));
			return default_value;
		}
	}

	void save() const;

private:
	std::map<std::string, std::string> contents;
	std::string filename;

	void remove_comment(std::string &line) const;		 ///< Removes comments at the end of the line
	bool only_whitespace(const std::string &line) const; ///< returns true if the whole line consists of only whitespace
	bool valid_line(const std::string &line) const;		 ///< Returns true if the line is a valid key=value line

	void extract_key(std::string &key, size_t const &sep_pos, const std::string &line) const;
	void extract_value(std::string &value, size_t const &sep_pos, const std::string &line) const;
	void extract_contents(const std::string &line);

	void parse_line(const std::string &line, size_t const line_number);

	void parse();
};

#endif /* CONFIGPARSER_H_ */
