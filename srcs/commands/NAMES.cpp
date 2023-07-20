#include "../../incs/ircserv.h"

strings_vec NAMES(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;
    std::string names;

    if (msg.params.size() < 1)
    {
        replies.push_back(ERR_NEEDMOREPARAMS(data, client, "NAMES"));
        return (replies);
    }
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "NAMES")));
    return NAMES(data, client, msg.params[0]);
}

strings_vec NAMES(t_data *data, Client *client, std::string channelName)
{
    strings_vec replies;
    std::string names;

    Channel *channel = Channel::findChannel(channelName);
    if (!channel)
    {
        replies.push_back(ERR_NOSUCHCHANNEL(data, client, channelName));
        return (replies);
    }

    std::vector<Client *> clients = channel->getClients();
    bool clientInTheChannel = channel->findUser(client->getNick()) != NULL;
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (!clientInTheChannel && clients[i]->flag_invisible)
            // If the client is not in the channel, we won't return invisible users.
            continue;
        if (channel->isOperator(clients[i]))
            names += "@" + clients[i]->getFullName() + " ";
        else if (channel->isVoiced(clients[i]))
            names += "+" + clients[i]->getFullName() + " ";
        else
            names += clients[i]->getFullName() + " ";
    }

    replies.push_back(error_rpl(data, client, "= " + channelName, "353", names));
    replies.push_back(error_rpl(data, client, channelName, "366", "End of /NAMES list."));
    return (replies);
}
