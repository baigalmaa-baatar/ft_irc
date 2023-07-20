#include "../../incs/ircserv.h"

strings_vec MOTD(t_data *data, Client *client, t_msg &msg)
{
	strings_vec replies;

	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "MOTD")));
	if (!msg.params.empty())
		return (replies);

	replies.push_back(RPL_MOTDSTART(data, client));
	replies.push_back(RPL_MOTD(data, client));
	replies.push_back(RPL_ENDOFMOTD(data, client));
	return (replies);
}
