#include "../../incs/ircserv.h"

strings_vec TOPIC(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;

    if (msg.params.empty())
    {
        // ERR_NEEDMOREPARAMS
        replies.push_back(error_rpl(data, client, "TOPIC", "461", "Not enough parameters"));
        return (replies);
    }
	if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "TOPIC")));

    Channel *channel = Channel::findChannel(msg.params[0]);

    if (channel == NULL)
    {
        // ERR_NOSUCHCHANNEL
        replies.push_back(error_rpl(data, client, msg.params[0], "403", "No such channel"));
        return (replies);
    }

    if (msg.params.size() == 1)
        return GET_TOPIC(data, client, msg.params[0]);

    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED
        replies.push_back(error_rpl(data, client, msg.params[0], "482", "You're not channel operator"));
        return (replies);
    }

    channel->setTopic(msg.params[1]);
    channel->sendAll(client->getFullName(), msg.raw, false);

    return (replies);
}

strings_vec GET_TOPIC(t_data *data, Client *client, std::string channelName)
{
    strings_vec replies;
    Channel *channel = Channel::findChannel(channelName);

    if (channel == NULL)
    {
        // ERR_NOSUCHCHANNEL
        replies.push_back(error_rpl(data, client, channelName, "403", "No such channel"));
        return (replies);
    }

    // asking channel topic
    std::string topic = channel->getTopic();
    if (topic.empty())
    {
        // RPL_NOTOPIC
        replies.push_back(error_rpl(data, client, channelName, "331", "No topic is set"));
        return (replies);
    }
    // RPL_TOPIC
    replies.push_back(error_rpl(data, client, channelName, "332", topic));
    return (replies);
}
