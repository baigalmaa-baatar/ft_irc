#include "../../incs/ircserv.h"

Channel *create_channel(std::string channel_name, Client *client)
{
    Channel *channel = new Channel(channel_name);
    channel->setMode("");
    channel->setTopic("");
    channel->flag_key = false;
    channel->flag_channel_user_limit = false;
    channel->addOperator(NULL, client);
    g_channels.push_back(channel);
    return (channel);
}

strings_vec JOIN(t_data *data, Client *client, t_msg &msg)
{
    /*
    JOIN #foobar                    ; join channel #foobar.
    JOIN &foo fubar                 ; join channel &foo using key "fubar".
    JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
    JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar" and channel #bar using key "foobar".
    JOIN #foo,#bar                  ; join channels #foo and #bar.
                                                            _
    ’#’->   Standard channels                                | 
    ’&’->   Local to the server where they are created      _|
    */

    strings_vec params = msg.params;
    strings_vec replies;
    if (params.size() == 0)
    {
        replies.push_back(ERR_NEEDMOREPARAMS(data, client, "JOIN"));
        return (replies);
    }

    if (!client->isRegistered())
        return (strings_vec(1, ERR_NOTREGISTERED(data, client, "JOIN")));

    // need to check, if parameters are more than 2 -> /join #chan1 pass1 fdasfd ? irssi is elimenating automatically

    std::stringstream ss_c(params[0]);
    strings_vec keys;
    if (params.size() > 1) // params[1].length())
    {
        std::stringstream ss_k(params[1]);
        while (ss_k.good())
        {
            std::string substr;
            getline(ss_k, substr, ',');
            keys.push_back(substr);
        }
    }

    strings_vec channel_names;
    while (ss_c.good())
    {
        std::string substr;
        getline(ss_c, substr, ',');
        if (!(substr[0] == '#' || substr[0] == '&' || substr.length() > 64))
        {
            // ERR_NOSUCHCHANNEL
            replies.push_back(error_rpl(data, client, substr, "403", "No such channel"));
            continue;
        }
        else
            channel_names.push_back(substr);
    }
    for (std::size_t i = 0; i < channel_names.size(); i++)
    {
        Channel *channel = NULL;
        channel = Channel::findChannel(channel_names[i]);
        if (!channel)
            channel = create_channel(channel_names[i], client);
        // check if channel key has set and key is correct or not
        if (channel->flag_key)
        {
            if ((params.size() > 1 && !(channel->getKey() == keys[i] || channel->getKey() == keys[0])) || params.size() == 1)
            {
                replies.push_back(error_rpl(data, client, channel_names[i], "475", "Cannot join channel (+k)"));
                continue;
            }
        }

        // check if user is already in channel
        if (channel->findUser(client->getNick()) != NULL)
        {
            replies.push_back(error_rpl(data, client, channel_names[i], "443", "is already on channel"));
            continue;
        }

        // check if channel has user numbers' limit
        if (channel->flag_channel_user_limit && channel->getClients().size() >= channel->chan_user_limit)
        {
            replies.push_back(error_rpl(data, client, channel_names[i], "471", "Cannot join channel (+l)"));
            continue;
        }

        // check if channel set invite only mode set
        if (channel->flag_invite_only && !channel->isInvited(client))
        {
            replies.push_back(error_rpl(data, client, channel_names[i], "473", "Cannot join channel (+i)"));
            continue;
        }

        // add client to channel
        channel->addClientToChannel(client);

        channel->sendAll(client->getFullName(), "JOIN :" + channel_names[i], false);
        if (client->getOperatorMode())
            channel->addOperator(NULL, client);

        strings_vec topic_replies = GET_TOPIC(data, client, channel_names[i]);
        strings_vec names_replies = NAMES(data, client, channel_names[i]);
        replies.insert(replies.end(), topic_replies.begin(), topic_replies.end());
        replies.insert(replies.end(), names_replies.begin(), names_replies.end());
    }
    return (replies);
}