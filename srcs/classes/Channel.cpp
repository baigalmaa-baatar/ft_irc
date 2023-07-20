#include "../../incs/Channel.hpp"

/************* CONSTRUCTORS AND DESTRUCTOR *************/

Channel::Channel() : _chan_name(), _topic(), _mode(), _clients(), _key(), chan_user_limit(), flag_key(), flag_channel_user_limit(), flag_invite_only(), flag_moderated() {}

Channel::Channel(const std::string name) : _chan_name(name), _topic(""), _mode(""), _clients(),  _key(""), chan_user_limit(1000), flag_key(false), flag_channel_user_limit(false), flag_invite_only(false), flag_moderated(false)
{
}

Channel::~Channel() {}

/********************** OPERATORS **********************/

Channel const &Channel::operator=(const Channel &rhs)
{
    this->_chan_name = rhs._chan_name;
    this->_topic = rhs._topic;
    this->_mode = rhs._mode;
    this->_clients = rhs._clients;
    this->_key = rhs._key;
    this->flag_key = rhs.flag_key;
    this->chan_user_limit = rhs.chan_user_limit;
    this->flag_channel_user_limit = rhs.flag_channel_user_limit;
    this->flag_channel_user_limit = rhs.flag_invite_only;
    this->flag_moderated = rhs.flag_moderated;
    return (*this);
}

/*********************** GETTERS ***********************/

const std::string &Channel::getChanName() const { return _chan_name; };
const std::string &Channel::getTopic() const { return _topic; };
const std::string &Channel::getMode() const { return _mode; };
std::vector<Client *> &Channel::getClients() { return _clients; };
const std::string &Channel::getKey() const { return _key; };

/*********************** SETTERS ***********************/

void Channel::setChanName(const std::string &name) { _chan_name = name; }
void Channel::setMode(const std::string &mode) { _mode = mode; }
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setChannelKey(const std::string &key)
{
    if (!getKey().length())
        _key = key;
    flag_key = true;
}

void Channel::setChannelLimit(const size_t limit) { chan_user_limit = limit; }
void Channel::setChannelInviteOnly(void) { flag_invite_only = true; }
void Channel::setChannelModerated(void) { flag_moderated = true; }
void Channel::addClientToChannel(Client * client)
{
    this->_clients.push_back(client);
}

void Channel::removeUser(std::string nick, bool notify)
{
    Client *user = findUser(nick);
    if (user == NULL)
        return ;

    removeOperator(NULL, user, notify);
    removeVoiced(NULL, user, notify);
    removeInvited(user);

    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        if ((*it)->getNick() == nick)
        {
            _clients.erase(it);
            break;
        }
}

bool Channel::addOperator(Client *client, Client *user)
{
    if (!add_into_list(_operator, user))
        return (false);

    sendAll(client == NULL ? "*" : client->getFullName(), "MODE " + _chan_name + " +o :" + user->getNick(), true);
    return (true);
}

bool Channel::removeOperator(Client *client, Client *user, bool notify)
{
    if (!remove_from_list(_operator, user))
        return (false);
    if (notify)
        sendAll(client == NULL ? "*" : client->getFullName(), "MODE " + _chan_name + " -o :" + user->getNick(), true);
    return (true);
}

bool Channel::isOperator(Client *user)
{
    return (in_list(_operator, user));
}

bool Channel::addVoiced(Client *client, Client *user)
{
    if (!add_into_list(_voiced, user))
        return (false);

    sendAll(client == NULL ? "*" : client->getFullName(), "MODE " + _chan_name + " +v :" + user->getNick(), true);
    return (true);
}

bool Channel::removeVoiced(Client *client, Client *user, bool notify)
{
    if (!remove_from_list(_voiced, user))
        return (false);
    if (notify)
        sendAll(client == NULL ? "*" : client->getFullName(), "MODE " + _chan_name + " -v :" + user->getNick(), true);
    return (true);
}

bool Channel::isVoiced(Client *user)
{
    return (in_list(_voiced, user));
}

bool Channel::addInvited(Client *user)
{
    return (add_into_list(_invited, user));
}

bool Channel::removeInvited(Client *user)
{
    return (remove_from_list(_invited, user));
}

bool Channel::isInvited(Client *user)
{
    return (in_list(_invited, user));
}

void Channel::sendChanOps(std::string sender, std::string msg)
{
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_operator[i]->getNick() == sender)
            continue;
        send_replies(strings_vec(1, ":" + sender + " " + msg + "\r\n"), _operator[i]);
    }
}

void Channel::sendAll(std::string sender, std::string msg, bool ignore_self)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFullName() == sender && ignore_self)
            continue;
        send_replies(strings_vec(1, ":" + sender + " " + msg + "\r\n"), _clients[i]);
    }
}

void Channel::sendAll(Client *client, std::vector<std::string> &others, std::string rpl, bool ignore_self)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNick() == client->getNick() && ignore_self)
            continue;
		bool already_sent = false;
		for (size_t j = 0; j < others.size(); j++)
		{
			if (_clients[i]->getNick() == others[j])
			{
				already_sent = true;
				break ;
			}
		}
		if (!already_sent)
		{
			others.push_back(_clients[i]->getNick());
			send_replies(strings_vec(1, rpl), _clients[i]);
		}
    }
}

void Channel::removeChannelKey(void)
{
    if (getKey().length())
        _key = "";
    flag_key = false;
}

void Channel::removeChannelUserLimit(void)
{
    chan_user_limit = 1000; //default channel user number ?
    flag_channel_user_limit = false;
}

Channel *Channel::findChannel(std::string chan_name)
{
    std::vector<Channel *>::iterator first = g_channels.begin();
    std::vector<Channel *>::iterator last = g_channels.end();
    while (first != last)
    {
        if ((*first)->getChanName() == chan_name)
            return (*first);
        ++first;
    }
    return (0);
}

Client *Channel::findUser(std::string nick)
{
    for (size_t i = 0; i < _clients.size(); i++)
        if (_clients[i]->getNick() == nick)
            return _clients[i];

    return NULL;
}

void Channel::removeChannelInviteOnly(void) { flag_invite_only = false; }
void Channel::removeChannelModerated(void)  { flag_moderated = false; }
