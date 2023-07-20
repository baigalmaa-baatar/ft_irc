#include "../../incs/ircserv.h"

//receiving pong when sending ping to clients -> do nothing if good param? else?
strings_vec PONG(t_data *data, Client *client, t_msg &msg)
{
	if (msg.params.empty())
		return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "PONG")));
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "PONG")));
	if (msg.params[0].empty() || (msg.params.size() > 1 && msg.params[1].empty()))
		return (strings_vec(1, ERR_NOORIGIN(data, client)));
	return (strings_vec());
}

//received ping -> answering pong
strings_vec PONG(t_data * data, t_msg &msg)
{
	std::string rep = ":" + data->servername + " PONG " \
					+ (msg.params.size() > 1 ? msg.params[1] : data->servername) \
					+ " :" + msg.params[0] + "\r\n";
	return (strings_vec(1, rep));
}