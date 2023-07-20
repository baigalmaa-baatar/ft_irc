#include "../../incs/ircserv.h"

strings_vec KILL(t_data *data, Client *client, t_msg &msg)
{
	strings_vec params = msg.params;

	if (!client->flag_oper)
		return (strings_vec(1, ERR_NOPRIVILEGES(data, client)));
	if (params.size() < 2)
		return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "KILL")));

	Client *ptr = findClient(data, msg.params[0]);
	if (!ptr || (ptr && client->getNick() == ptr->getNick()))
		return (strings_vec(1, ERR_NOSUCHNICK(data, client, msg.params[0])));

	msg.params[0] = client->getNick();
	return (QUIT(data, ptr, msg));
}
