#include "../../incs/ircserv.h"

//received ping -> answering pong
strings_vec PING(t_data *data, Client *client, t_msg &msg)
{
	if (msg.params.empty())
		return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "PING")));
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "PING")));
	if (msg.params[0].empty() || (msg.params.size() > 1 && msg.params[1].empty()))
		return (strings_vec(1, ERR_NOORIGIN(data, client)));
	return (PONG(data, msg));
}

//sending ping (not used yet)
strings_vec PING()
{
	return (strings_vec(1, "PING :random\r\n"));
}
