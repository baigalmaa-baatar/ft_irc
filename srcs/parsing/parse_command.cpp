#include "../../incs/ircserv.h"

std::string capitalize(std::string &s)
{
	std::string x;
	for (std::size_t i = 0; i < s.size(); i++)
	{
		char c = (s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i];
		x.push_back(c);
	}
	return (x);
}

std::string parse_command(std::string &command)
{
	std::string parsed(command);
	if (starts("/", command))
		parsed = command.substr(1);
	return (capitalize(parsed));
}
