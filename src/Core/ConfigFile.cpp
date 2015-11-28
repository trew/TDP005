#include <Core/ConfigFile.h>
#include <ctime>

namespace ConfigUtil
{
	void warn(const std::string &error)
	{
		std::cout << "CFG - Warning: " << error << std::endl;
	}
}

ConfigFile::ConfigFile(const std::string &fn): filename(fn)
{
	parse();
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::removeComment(std::string &line) const
{
	if (line.find('#') != line.npos)
	{
		line.erase(line.find('#'));
	}
}

bool ConfigFile::isOnlyWhitespace(const std::string &line) const
{
	return line.find_first_not_of(' ') == line.npos;
}

bool ConfigFile::isValidLine(const std::string &line) const
{
	// remember to run removeComment() before this function!

	std::string tmp = line;
	tmp.erase(0, tmp.find_first_not_of("\t "));
	if (tmp[0] == '=') // no key was found
	{
		return false;
	}

	// look for value
	for (size_t i = tmp.find('=') + 1; i < tmp.length(); i++)
	{
		if (tmp[i] != ' ')
		{
			return true;
		}
	}

	// only whitespace was found
	return false;
}

void ConfigFile::extractKey(std::string &key, const size_t &sep_pos, const std::string &line) const
{
	key = line.substr(0, sep_pos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos)
	{
		key.erase(key.find_first_of("\t "));
	}
}

void ConfigFile::extractValue(std::string &value, const size_t &sep_pos, const std::string &line) const
{
	value = line.substr(sep_pos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extractContents(const std::string &line)
{
	// called from parse_line(), validated there

	std::string tmp = line;
	// remove leading whitespace
	tmp.erase(0, tmp.find_first_not_of("\t "));
	size_t sep_pos = tmp.find('=');

	std::string key, value;
	extractKey(key, sep_pos, tmp);
	extractValue(value, sep_pos, tmp);

	if (!containsKey(key))
	{
		contents.insert(std::pair<std::string, std::string>(key, value));
	}
	else
	{
		ConfigUtil::warn(key + " was declared at least twice in configuration file.");
	}
}

void ConfigFile::parseLine(const std::string &line, size_t const line_number)
{
	// validate line
	if (line.find('=') == line.npos)
	{
		ConfigUtil::warn("Cannot find separator on line: " + Convert::toString(line_number) + "\n");
		return;
	}
	if (!isValidLine(line))
	{
		ConfigUtil::warn("Bad format on line: " + Convert::toString(line_number) + "\n");
		return;
	}

	extractContents(line);
}

bool ConfigFile::containsKey(const std::string &key) const
{
	return contents.find(key) != contents.end();
}

void ConfigFile::parse()
{
	std::ifstream file;
	file.open(filename.c_str());
	if (!file)
	{
		ConfigUtil::warn("File " + filename + " could not be found.");
		return;
	}

	std::string line;
	size_t line_number = 0;
	while (std::getline(file, line))
	{
		line_number++;
		std::string tmp = line;
		if (tmp.empty() || tmp == "\r" || tmp == "\n")
		{
			continue;
		}

		removeComment(tmp);
		if (isOnlyWhitespace(tmp))
		{
			continue;
		}

		parseLine(tmp, line_number);
	}

	file.close();
}

void ConfigFile::save() const
{
	std::ofstream file;
	file.open(filename.c_str());
	if (!file)
	{
		ConfigUtil::warn("File " + filename + " could not be opened.");
		return;
	}

	// TODO localtime_s is windows-only.
	time_t t = time(0); //time now
	struct tm timeinfo;
	char buffer[80];

	time ( &t );
	localtime_s(&timeinfo, &t);
	strftime (buffer, 80, "%Y-%m-%d %X", &timeinfo);

	file << "# Configuration file" << std::endl;
	file << "# " << buffer << std::endl << std::endl;

	for (std::map<std::string, std::string>::const_iterator it = contents.begin(); it != contents.end(); it++)
	{
		file << (*it).first << "=" << (*it).second << std::endl;
	}

	file.close();
}
