#ifndef CONFIGPARSER_H_
#define CONFIGPARSER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <stdexcept>

namespace ConfigUtil
{
	void warn(const std::string &error);
}

/**
 * http://www.dreamincode.net/forums/topic/183191-create-a-simple-configuration-file-parser/
 */
class Convert
{
public:
	class ParseException: public std::runtime_error
	{
	public:
		ParseException(const std::string &w) : std::runtime_error(w) {}
	};

	template<typename T> static std::string toString(const T &val)
	{
		std::ostringstream os;
		os << val;
		return os.str();
	}

	template<typename T> static T fromString(const std::string &val)
	{
		std::istringstream istr(val);
		T returnVal;
		if (!(istr >> returnVal))
		{
			throw ParseException("CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n");
		}

		return returnVal;
	}
};

class ConfigFile
{
public:
	ConfigFile(const std::string &filename);
	virtual ~ConfigFile();

	bool containsKey(const std::string &key) const;

	template <typename V> V getValue(const std::string &key, V const &defaultValue = V()) const
	{
		if (!containsKey(key))
		{
			return defaultValue;
		}
		try
		{
			return Convert::fromString<V>(contents.find(key)->second);
		}
		catch (Convert::ParseException &e)
		{
			ConfigUtil::warn(std::string(e.what()));
			return defaultValue;
		}
	}

	template <typename V> void setValue(const std::string &key, V const &value)
	{
		contents[key] = Convert::toString(value);
	}

	void save() const;

private: // Methods
	/**
	 * Removes comments at the end of the line
	 */
	void removeComment(std::string &line) const;

	/**
	 * Returns true if the whole line consists of only whitespace
	 */
	bool isOnlyWhitespace(const std::string &line) const;

	/**
	 * Returns true if the line is a valid key=value line
	 */
	bool isValidLine(const std::string &line) const;

	void extractKey(std::string &key, const size_t &sep_pos, const std::string &line) const;
	void extractValue(std::string &value, const size_t &sep_pos, const std::string &line) const;
	void extractContents(const std::string &line);

	void parseLine(const std::string &line, const size_t line_number);

	void parse();

private: // Properties
	std::map<std::string, std::string> contents;
	std::string filename;

};

#endif /* CONFIGPARSER_H_ */
