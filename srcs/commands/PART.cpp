#include "../../incs/ircserv.h"

strings_vec PART(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;

	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "PART")));

    for (size_t i = 0; i < msg.params.size(); i++)
    {
        Channel *channel = Channel::findChannel(msg.params[i]);
        if (channel == NULL)
        {
            replies.push_back(error_rpl(data, client, msg.params[i], "403", "No such channel"));
            continue;
        }

        if (channel->findUser(client->getNick()) == NULL)
        {
            replies.push_back(error_rpl(data, client, msg.params[i], "442", "You're not on that channel"));
            continue;
        }

        channel->sendAll(client->getFullName(), "PART :" + msg.params[i], false);
        channel->removeUser(client->getNick(), false);
    }

    return (replies);
}
