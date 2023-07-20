#include "../../incs/ircserv.h"

strings_vec NICK(t_data *data, Client *client, t_msg &msg)
{
	strings_vec replies;
	strings_vec params = msg.params;

	if (params.empty()) // not enough params
		replies.push_back(ERR_NEEDMOREPARAMS(data, client, "NICK"));
	else if (params[0].empty()) // no nick given
		replies.push_back(ERR_NONICKNAMEGIVEN(data, client));
	else
	{
		std::string new_nick = params[0];
		if (findClient(data, new_nick) && client->getNick() != new_nick) // nick already used by another client
			replies.push_back(ERR_NICKNAMEINUSE(data, client, new_nick));
		else if (contains(" ,*?!@.$:#&", new_nick) || new_nick.size() > 30) // nick containing prohibited characters or too long
			replies.push_back(ERR_ERRONEUSNICKNAME(data, client, new_nick));
		else // nick ok
		{
			if (client->getNick().empty()) // setting nick for the first time
				client->nickCall();
			else if (client->getNick() != new_nick && client->isRegistered()) // changing nick if different from current
				replies.push_back(user_rpl(client, "NICK", new_nick));

			// informs all channel the user joined that the user has changed their nick name
			for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end(); ++it)
				if ((*it)->findUser(client->getNick()) != NULL)
					(*it)->sendAll(client->getFullName(), msg.raw, true);

			client->setNick(new_nick);
		}
	}
	return (replies);
}
