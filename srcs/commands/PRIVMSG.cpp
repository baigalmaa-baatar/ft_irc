#include "../../incs/ircserv.h"

std::string sendMessageToUser(t_data *data, Client *client, std::string command, std::string user, std::string msg)
{
    std::string nick = matchNames(data, user);
    if (!nick.size())
        return (error_rpl(data, client, nick, "401", "No such nick/channel"));

    Client *receiver = findClient(data, nick);
    if (receiver == NULL)
        return (error_rpl(data, client, nick, "401", "No such nick/channel"));

    send_replies(strings_vec(1, ":" + client->getNick() + " " + command + " " + user + " :" + msg + "\r\n"), receiver);
    return ("");
}

std::string sendMessageToMask(t_data *data, Client *client, std::string command, std::string mask, std::string msg)
{
    size_t pos = mask.find(".");
    if (pos == std::string::npos)
        return (error_rpl(data, client, mask, "401", "No such nick/channel"));
    strings_vec replies;
    bool flag = false;

    for (size_t i = 1; i < data->clients.size(); i++)
        if (matchWildcard(data->clients[i]->getFullName(), mask)) //#*atar!bbaatar@127.0.0.1
        {
            sendMessageToUser(data, client, command, data->clients[i]->getNick(), msg);
            flag = true;
        }

    if (!flag)
        return (error_rpl(data, client, mask, "401", "No such nick/channel"));

    return ("");
}

std::string sendMessageToChanOps(t_data *data, Client *client, std::string command, std::string target, std::string msg)
{
    std::string chan_name = target;
    chan_name.erase(0, 1);

    Channel *channel = NULL;
    channel = Channel::findChannel(chan_name);
    if (channel == NULL)
        return (error_rpl(data, client, chan_name, "403", "No such channel"));

    if (channel->findUser(client->getNick()) == NULL)
        return (error_rpl(data, client, chan_name, "404", "Cannot send to channel"));

    channel->sendChanOps(client->getNick(), "" + command + " " + target + " :" + msg);

    return ("");
}

std::string sendMessageToChannel(t_data *data, Client *client, std::string command, std::string chan_name, std::string msg)
{
    Channel *channel = NULL;
    channel = Channel::findChannel(chan_name);
    if (channel == NULL)
        return (error_rpl(data, client, chan_name, "403", "No such channel"));

    if (channel->findUser(client->getNick()) == NULL)
        return (error_rpl(data, client, chan_name, "404", "Cannot send to channel"));

    if (channel->flag_moderated && !channel->isOperator(client) && !channel->isVoiced(client))
    {
        /**
         *  if channel set moderated mode (+m),
            1. chanops
            2. users who has +v mode have can send message
        */
        return (error_rpl(data, client, chan_name, "404", "Cannot send to channel"));
    }

    channel->sendAll(client->getFullName(), "" + command + " " + chan_name + " :" + msg, true);
    return ("");
}

strings_vec PRIVMSG(t_data *data, Client *client, t_msg &msg)
{
    /*
    case 1: just nick name  ok
    PRIVMSG Angel :yes I'm receiving it !
                                   ; Command to send a message to Angel.

    case 2: user name and host name ok
    PRIVMSG jto@tolsun.oulu.fi :Hello !
                                   ; Command to send a message to a user
                                   on server tolsun.oulu.fi with
                                   username of "jto".
    case 3: full name ok
    PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
                                   ; Message to the user with nickname
                                   Wiz who is connected from the host
                                   tolsun.oulu.fi and has the username
                                   "jto".
    4.wildcard(mask) to everyone ok
    PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                   ; Message to everyone on a server
                                   which has a name matching *.fi.

    PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                   ; Message to all users who come from
                                   a host which has a name matching
                                   *.edu.
    5. channel operators ok
    PRIVMSG %#bunny :Hi! I have a problem!
                                  ; Command to send a message to halfops
                                  and chanops on #bunny.

    PRIVMSG @%#bunny :Hi! I have a problem!
                                  ; Command to send a message to halfops
                                  and chanops on #bunny. This command is
                                  functionally identical to the above
                                  command.
    */
   
    strings_vec params = msg.params;
    strings_vec replies;

    if (params.size() < 2)
        return (strings_vec(1, ERR_NEEDMOREPARAMS(data, client, msg.command)));
    if (!client->isRegistered())
        return (strings_vec(1, ERR_NOTREGISTERED(data, client, "PRIVMSG")));
   
    strings_vec targets = split(params[0], ",");
    std::string message;
    for (std::size_t i = 1; i < msg.params.size(); i++)
			message += (i != msg.params.size() - 1 ? (msg.params[i] + " ") : msg.params[i]);
    for (strings_vec::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        std::string target = *it;

        if (target.empty())
            continue;
        if (target[0] == '#')
        {
            // channel or host mask
            if (target.find('*') == std::string::npos)
            {
                // doesn't contain '*', so it's a channel
                replies.push_back(sendMessageToChannel(data, client, msg.command, target, message));
            }
            else
            {
                // contains '*', so it's a host mask
                replies.push_back(sendMessageToMask(data, client, msg.command, target.erase(0, 1), message));
            }
        }
        else if (target[0] == '$')
        {
            // server mask
            replies.push_back(sendMessageToMask(data, client, msg.command, target.erase(0, 1), message));
        }
        else if (target[0] == '%' || target[0] == '@')
        {
            // channel operators
            replies.push_back(sendMessageToChanOps(data, client, msg.command, target, message));
        }
        else
        {
            // nick name + username@hostname + fullname
            replies.push_back(sendMessageToUser(data, client, msg.command, target, message));
        }
    }

    if (msg.command == "NOTICE")
        // we don't reply anything for NOTICE command-> we don't open new window
        return strings_vec();

    return (replies);
}
