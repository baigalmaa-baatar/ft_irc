#include "../../incs/ircserv.h"

strings_vec USER(t_data *data, Client *client, t_msg &msg)
{
	strings_vec replies;
	strings_vec params = msg.params;
	if (params.size() < 4)
		return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "USER")));
	if (client->getName().empty())
	{
		client->userCall();
		client->setName(params[0]);
		client->setRealName(params[3]);
	}
	else
		return (strings_vec(1, ERR_ALREADYREGISTERED(data, client)));
	return (replies);
}
