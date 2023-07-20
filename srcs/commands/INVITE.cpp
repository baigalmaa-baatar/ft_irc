#include "../../incs/ircserv.h"

strings_vec INVITE(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;

    if (msg.params.size() < 2)
    {
        // ERR_NEEDMOREPARAMS
        replies.push_back(error_rpl(data, client, "INVITE", "461", "Not enough parameters"));
        return replies;
    }
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "INVITE")));

    Client *user = findClient(data, msg.params[0]);
    Channel *channel = Channel::findChannel(msg.params[1]);

    if (user == NULL)
    {
        // ERR_NOSUCHNICK
        replies.push_back(error_rpl(data, client, msg.params[0], "401", "No such nick/channel"));
        return replies;
    }

    if (channel == NULL)
    {
        // ERR_NOSUCHCHANNEL
        replies.push_back(error_rpl(data, client, msg.params[0], "403", "No such channel"));
        return replies;
    }
    /**
     * Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED
     * numeric. In particular, they SHOULD reject it when the channel
     * has invite-only mode set, and the user is not a channel operator.
    */
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED
        replies.push_back(error_rpl(data, client, msg.params[1], "482", "You're not channel operator"));
        return replies;
    }

    if (channel->findUser(msg.params[0]) != NULL)
    {
        // ERR_USERONCHANNEL
        replies.push_back(error_rpl(data, client, msg.params[0] + " " + msg.params[1], "443", "is already on channel"));
        return replies;
    }

    channel->addInvited(user);

    send_replies(strings_vec(1, ":" + client->getNick() + " " + msg.raw + "\r\n"), user);
    // RPL_INVITING
	replies.push_back(error_rpl(data, client, msg.params[0] + " " + msg.params[1], "341", ""));
    return replies;
}
