#include "../../incs/ircserv.h"

strings_vec ERROR(t_data *data, Client *client, t_msg &msg)
{
	(void)data;
	client->notifClose();
	std::string reason = "ERROR :Closing link: (" \
					+ client->getName() + "@" + client->getHost() + ") " \
					+ "[" + msg.command + ": " + msg.param + "]\r\n";
    send_replies(strings_vec(1, reason), client);
	return (strings_vec());
}
