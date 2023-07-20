#include "../incs/ircserv.h"

strings_vec split(std::string s, std::string sep)
{
	strings_vec words;
	std::size_t nb = -1;
	std::size_t previous = 0;
	while ((nb = s.find(sep, ++nb)) != std::string::npos)
	{
		std::string sub = s.substr(previous, nb - previous);
		if (!sub.empty())
			words.push_back(sub);
		previous = nb + 1;
	}
	if (!s.substr(previous).empty())
		words.push_back(s.substr(previous));
	return (words);
}

strings_vec separate_words(std::string s)
{
	return (split(s, " "));
}

strings_vec separate_lines(std::string s)
{
	strings_vec lines;
	std::size_t nb = 0;
	std::size_t store1 = 0;
	std::size_t store2 = 0;
	std::size_t previous = 0;
	while ((nb = s.find("\r\n", nb)) != std::string::npos
		|| (nb = s.find("\n", store1)) != std::string::npos
		|| (nb = s.find("\r", store2)) != std::string::npos)
	{
		lines.push_back(s.substr(previous, nb - previous));
		(s.substr(nb, 2) == std::string("\r\n")) ? previous = (nb += 2) : previous = ++nb;// +2 if \r\n, +1 if \n or \r
		store1 = nb;
		store2 = nb;
	}
	// if (lines.empty())
	// 	lines.push_back(s);
	return (lines);
}

bool contains(const char *chars, std::string s)
{
	if (s.empty())
		return (false);
	for (int i = 0; chars[i]; i++)
	{
		for (std::size_t j = 0; j < s.size(); j++)
		{
			if (s[j] == chars[i])
				return (true);
		}
	}
	return (false);
}

bool starts(const char *chars, std::string s)
{
	if (s.empty())
		return (false);
	for (int i = 0; chars[i]; i++)
	{
		if (s[0] == chars[i])
			return (true);
	}
	return (false);
}

bool ends(char *buffer)
{
	int i = 0;
	if (!buffer)
		return (false);
	while (buffer[i])
		i++;
	if (i > 1 && buffer[i - 1] == '\n' && buffer[i - 2] == '\r')
		return (true);
	if (i > 0 && (buffer[i - 1] == '\n' || buffer[i - 1] == '\r'))
		return (true);
	return (false);
}

bool in_list(std::vector<Client *> list, Client *client)
{
	for (std::vector<Client *>::iterator it = list.begin(); it != list.end(); ++it)
		if ((*it)->getNick() == client->getNick())
			return (true);

	return (false);
}

bool add_into_list(std::vector<Client *> &list, Client *client)
{
	if (in_list(list, client))
		// already exists, won't add again
		return (false);

	list.push_back(client);
	return (true);
}

bool remove_from_list(std::vector<Client *> &list, Client *client)
{
	for (std::vector<Client *>::iterator it = list.begin(); it != list.end(); ++it)
		if ((*it)->getNick() == client->getNick())
		{
			list.erase(it);
			return (true);
		}

	return (false);
}

Client *getClient(t_data *data, int fd)
{
	for (std::vector<Client*>::iterator it = data->clients.begin(); it != data->clients.end(); it++)
	{
		if (fd == (*it)->getFd())
			return (*it);
	}
	return (NULL);
}
