#include "../../incs/ircserv.h"

strings_vec OPER(t_data *data, Client *client, t_msg &msg)
{
	strings_vec params = msg.params;
	if (params.size() < 2)
		return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "OPER")));
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "OPER")));
	if (params[0] == data->oper[0])
	{
		if (params[1] == data->oper[1])
		{
			client->setOperatorMode();
			// makes operator in all channels
			for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end(); ++it)
				if ((*it)->findUser(client->getNick()) != NULL)
					(*it)->addOperator(NULL, client);

			return (strings_vec(1, RPL_YOUREOPER(data, client)));
		}
		else
			return (strings_vec(1, ERR_PASSWDMISMATCH(data, client, "OPER")));
	}
	return (strings_vec(1, ERR_NOOPERHOST(data, client)));
}
