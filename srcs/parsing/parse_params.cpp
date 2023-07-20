#include "../../incs/ircserv.h"

strings_vec parse_colons(strings_vec &params)
{
	for (std::size_t i = 0; i < params.size(); i++)
	{
		if (starts(":", params[i]))
			params[i] = params[i].substr(1);
	}
	return (params);
}

strings_vec parse_params(std::string &params)
{
	std::size_t pos = params.rfind(':');
	if (pos == std::string::npos)
		return (separate_words(params));

	strings_vec parsed;
	parsed = separate_words(params.substr(0, pos));
	parsed.push_back(params.substr(pos));
	return (parse_colons(parsed));
}