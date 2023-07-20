#include "../../incs/ircserv.h"

// expands the mode string
//
// Channel Mode:
// [+kml,password,1000] -> [[+k,password], [+m], [+l,1000]]
//
// User Mode:
// [-io] > [[-i], [-o]]

std::vector<strings_vec> expand(strings_vec input, std::string modes_with_params)
{
    size_t param_index = 1;
    std::vector<strings_vec> output;
    std::string sign = std::string(1, input[0][0]);

    for (size_t i = 1; i < input[0].length(); i++)
    {
        strings_vec mode;
        // bool flag_not_enough = false;

        mode.push_back(sign + input[0][i]);
        if (modes_with_params.find(input[0][i]) != std::string::npos)
        {
            // mode with a parameter
            if (param_index < input.size())
            {
                mode.push_back(input[param_index++]);
                // flag_not_enough = true;
            }
            else
                // not enough parameters, will return empty list
                return (std::vector<strings_vec>());
        }
        output.push_back(mode);
    }

    return (output);
}

std::string setChannelMode(t_data *data, Client *client, Channel *channel, strings_vec mode)
{
    if (mode[0][1] == 'k')
    {
        if (channel->flag_key)
            // ERR_KEYSET
            return (error_rpl(data, client, channel->getChanName(), "472", "Channel key already set"));

        channel->setChannelKey(mode[1]);

        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " +k " + mode[1], "324", ""));
    }

    if (mode[0][1] == 'l')
    {
        if (mode[1].find_first_not_of("0123456789") != std::string::npos)
            return (ERR_INVALIDMODEPARAM(data, client, "MODE"));

        std::stringstream sstream(mode[1]);
        size_t result;
        sstream >> result;
        channel->setChannelLimit(result);
        channel->flag_channel_user_limit = true;

        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " +l " + mode[1], "324", ""));
    }

    if (mode[0][1] == 'i')
    {
        // invite only channel
        if (channel->flag_invite_only)
            return ("");

        channel->setChannelInviteOnly();

        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " +i", "324", ""));
    }

    if (mode[0][1] == 'm')
    {
        // moderated channel
        if (channel->flag_moderated)
            return ("");

        channel->setChannelModerated();

        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " +m", "324", ""));
    }

    if (mode[0][1] == 'v')
    {
        /**
         * Set voice mode in moderated channel flag
         * /MODE #Finnish +v Wiz
         * ; Command to allow WiZ to speak on #Finnish.
         */
        Client *user = findClient(data, mode[1]);
        if (user == NULL)
            // ERR_NOSUCHNICK
            return (error_rpl(data, client, mode[1], "401", "No such nick/channel"));

        if (channel->addVoiced(client, user))
            // RPL_CHANNELMODEIS
            return (error_rpl(data, client, channel->getChanName() + " +v " + mode[1], "324", ""));
        else
            return ("");
    }

    if (mode[0][1] == 'o')
    {
        /**
         * /MODE #Finnish +o Kilroy
         * ; Command to give 'chanop' privileges to Kilroy on channel #Finnish.
         */
        Client *user = findClient(data, mode[1]);
        if (user == NULL)
            // ERR_NOSUCHNICK
            return (error_rpl(data, client, mode[1], "401", "No such nick/channel"));

        if (channel->addOperator(client, user))
            // RPL_CHANNELMODEIS
            return (error_rpl(data, client, channel->getChanName() + " +o " + mode[1], "324", ""));
        else
            return ("");
    }

    // ERR_UNKNOWNMODE
    return (error_rpl(data, client, std::string(1, mode[0][1]), "472", "is unknown mode char to me"));
}

std::string removeChannelMode(t_data *data, Client *client, Channel *channel, strings_vec mode)
{
    if (mode[0][1] == 'k')
    {
        if (!channel->flag_key)
            return ("");

        channel->removeChannelKey();
        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " -k", "324", ""));
    }

    if (mode[0][1] == 'l')
    {
        if (!channel->flag_channel_user_limit)
            return ("");

        channel->removeChannelUserLimit();
        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " -l", "324", ""));
    }

    if (mode[0][1] == 'i')
    {
        if (!channel->flag_invite_only)
            return ("");

        channel->removeChannelInviteOnly();
        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " -i", "324", ""));
    }

    if (mode[0][1] == 'm')
    {
        if (!channel->flag_moderated)
            return ("");

        channel->removeChannelModerated();
        // RPL_CHANNELMODEIS
        return (error_rpl(data, client, channel->getChanName() + " -m", "324", ""));
    }

    if (mode[0][1] == 'v')
    {
        /**
         * remove moderated channel flag
         * /MODE #Finnish +v Wiz
         * ; Command to allow WiZ to speak on #Finnish.
         */
        Client *user = findClient(data, mode[1]);
        if (user == NULL)
            // ERR_NOSUCHNICK
            return error_rpl(data, client, mode[1], "401", "No such nick/channel");

        if (channel->removeVoiced(client, user, true))
            // RPL_CHANNELMODEIS
            return (error_rpl(data, client, channel->getChanName() + " -v " + mode[1], "324", ""));
        else
            return ("");
    }

    if (mode[0][1] == 'o')
    {
        Client *user = findClient(data, mode[1]);
        if (user == NULL)
            // ERR_NOSUCHNICK
            return error_rpl(data, client, mode[1], "401", "No such nick/channel");

        if (channel->removeOperator(client, user, true))
            // RPL_CHANNELMODEIS
            return (error_rpl(data, client, channel->getChanName() + " -o " + mode[1], "324", ""));
        else
            return ("");
    }

    // ERR_UNKNOWNMODE
    return (error_rpl(data, client, std::string(1, mode[0][1]), "472", "is unknown mode char to me"));
}

strings_vec changeChannelMode(t_data *data, Client *client, std::string chan_name, strings_vec params)
{
    /**
     * /MODE #chan1 +k
     * /MODE #chan1 +l
     * /MODE #chan1 +i
     * /MODE #chan1 +m
     * /MODE #chan1 +v
    */
    strings_vec replies;
    Channel *channel = NULL;
    channel = Channel::findChannel(chan_name);

    if (!channel)
    {
        replies.push_back(error_rpl(data, client, chan_name, "403", "No such channel"));
        return (replies);
    }

    if (!channel->isOperator(client))
    {
        replies.push_back(error_rpl(data, client, chan_name, "482", "You're not channel operator"));
        return (replies);
    }

    std::vector<strings_vec> modes;

    if (params[0][0] == '+')
        modes = expand(params, "klvo");
    else
        modes = expand(params, "vo");

    if (modes.empty())
    {
        replies.push_back(ERR_NEEDMOREPARAMS(data, client, "MODE"));
        return (replies);
    }

    for (std::vector<strings_vec>::iterator it = modes.begin(); it != modes.end(); ++it)
    {
        strings_vec mode = *it;

        if (mode[0][0] == '+')
            replies.push_back(setChannelMode(data, client, channel, mode));
        else
            replies.push_back(removeChannelMode(data, client, channel, mode));
    }

    return (replies);
}

strings_vec changeUserMode(t_data *data, Client *client, std::string nick, strings_vec params)
{
    /**
     * /MODE dan +i //invisible
     * /MODE dan +o //operator
     */
    strings_vec replies;
    if (client->getNick() != nick)
    {
        // ERR_USERSDONTMATCH
        replies.push_back(error_rpl(data, client, nick, "502", "Cant change mode for other users"));
        return (replies);
    }
    else
    {
        if (params[0] == "+i")
        {
            // set invisible mode
            client->setInvisibleMode();
            // RPL_UMODEIS
            replies.push_back(error_rpl(data, client, params[0], "221", ""));
        }
        else if (params[0] == "+o")
        {
            /*
             If a user attempts to make themselves an operator using the "+o"
             flag, the attempt should be ignored.  There is no restriction,
             however, on anyone `deopping' themselves (using "-o").
             link: https://www.rfc-editor.org/rfc/rfc1459#section-4.2.3.2
            */
        }
        else if (params[0] == "-i")
        {
            // remove invisible mode
            client->removeInvisibleMode();
            // RPL_UMODEIS
            replies.push_back(error_rpl(data, client, params[0], "221", ""));
        }
        else if (params[0] == "-o")
        {
            // remove operator mode
            client->removeOperatorMode();

            // removes from operator in all channels
			for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end(); ++it)
				if ((*it)->findUser(client->getNick()) != NULL)
					(*it)->removeOperator(client, client, true);

            // RPL_UMODEIS
            replies.push_back(error_rpl(data, client, params[0], "221", ""));
        }
        else
        {
            // ERR_UMODEUNKNOWNFLAG
            replies.push_back(error_rpl(data, client, nick, "501", "Unknown MODE flag"));
            return (replies);
        }
    }
    return (replies);
}

std::string getChannelMode(t_data *data, Client *client, std::string chan_name)
{
    std::string mode = "+";
    Channel *channel = NULL;
    channel = Channel::findChannel(chan_name);

    if (!channel)
        return ERR_NOSUCHCHANNEL(data, client, chan_name);

    if (channel->flag_invite_only)
        mode += 'i';

    if (channel->flag_moderated)
        mode += 'm';

    if (channel->flag_key)
        mode += 'k';

    if (channel->flag_channel_user_limit)
        mode += 'l';

    if (mode != "+")
        // RPL_UMODEIS
        return (error_rpl(data, NULL, chan_name, "324", mode));

    return ("");
}

std::string getUserMode(t_data *data, Client *client, std::string nick)
{
    std::string mode = "+";
    if (client->getNick() != nick)
        return (error_rpl(data, client, "", "502", "Can't view modes for other users"));

    if (client->flag_invisible)
        mode += 'i';

    if (client->flag_oper)
        mode += 'o';

    if (mode != "+")
        // RPL_CHANNELMODEIS
        return (error_rpl(data, NULL, nick, "324", mode));
    return ("");
}

strings_vec MODE(t_data *data, Client *client, t_msg &msg)
{
    strings_vec params = msg.params;
    if (params.empty() || params[0].empty())
        return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "MODE")));

    if (params.size() == 1)
    {
        if (params[0][0] == '#')
            return (strings_vec(1, getChannelMode(data, client, params[0])));
        else
            return (strings_vec(1, getUserMode(data, client, params[0])));
    }

    if (!client->isRegistered())
		return (strings_vec(1, ERR_NOTREGISTERED(data, client, "MODE")));
    
    std::string target = params[0];
    params.erase(params.begin());

    if (target.empty() || params[0].empty())
        return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, "MODE")));

    if (params[0][0] != '+' && params[0][0] != '-')
        return (strings_vec(1, ERR_INVALIDMODEPARAM(data, client, "MODE")));

    if (target[0] == '&' || target[0] == '#')
        return (changeChannelMode(data, client, target, params));

    return (changeUserMode(data, client, target, params));
}
