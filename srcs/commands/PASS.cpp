#include "../../incs/ircserv.h"

strings_vec PASS(t_data *data, Client *client, t_msg &msg)
{
	strings_vec replies;
	strings_vec params = msg.params;
	if (!params.empty())
	{
		if (client->isRegistered())
			replies.push_back(ERR_ALREADYREGISTERED(data, client, "PASS"));
		else if (params[0] != data->password)
			replies.push_back(ERR_PASSWDMISMATCH(data, client, "PASS"));
		else if (params[0] == data->password)
			client->passCall();
	}
	else
		replies.push_back(ERR_NEEDMOREPARAMS(data, client, "PASS"));
	return (replies);
}
