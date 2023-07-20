#include "../../incs/ircserv.h"

strings_vec KICK(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;

    if (msg.params.size() < 2)
    {
        // ERR_NEEDMOREPARAMS
        replies.push_back(error_rpl(data, client, "KICK", "461", "Not enough parameters"));
        return replies;
    }
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "KICK")));

    Channel *channel = Channel::findChannel(msg.params[0]);
    Client *user = findClient(data, msg.params[1]);

    if (channel == NULL)
    {
        // ERR_NOSUCHCHANNEL
        replies.push_back(error_rpl(data, client, msg.params[0], "403", "No such channel"));
        return replies;
    }
    if (channel->findUser(msg.params[1]) == NULL)
    {
        // ERR_USERNOTINCHANNEL
        replies.push_back(error_rpl(data, client, msg.params[1] + " " + msg.params[0], "441", "They aren't on that channel"));
        return replies;
    }
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED
        replies.push_back(error_rpl(data, client, msg.params[0], "482", "You're not channel operator"));
        return replies;
    }

    channel->sendAll(client->getFullName(), msg.raw, false);
    channel->removeUser(user->getNick(), false);

    return replies;
}
