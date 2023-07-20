#include "../../incs/ircserv.h"

void showChan(std::string color = "RE", std::string s = "***********DEBUG***********")
{
    std::cout << color << "------" << s << "-----" << NC << std::endl;
    for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end(); it++)
    {
        std::cout << RE << "Channels are : " << (*it)->getChanName() << NC << std::endl;
        std::vector<Client *> tmp = (*it)->getClients();
        for (size_t i = 0; i < tmp.size(); i++)
            std::cout << "In this channel we have these clients: " << tmp[i]->getNick() << NC << std::endl;

        std::cout << "*** Channel mode information ***" << (*it)->flag_key << std::endl;

        std::cout << "channel KEY flag is : " << (*it)->flag_key << std::endl;
        if ((*it)->flag_key)
            std::cout << "Channel password is : " << (*it)->getKey() << std::endl;
        std::cout << "Channel user limit flag is : " << (*it)->flag_channel_user_limit << std::endl;
        if ((*it)->flag_channel_user_limit)
            std::cout << "Channel user limit is : " << (*it)->chan_user_limit << std::endl;
        std::cout << "Channel invite flag is : " << (*it)->flag_invite_only << std::endl;
    }
    std::cout << color << "----------------------" << NC << std::endl;
}

strings_vec DEBUG(t_data *data, Client *client, t_msg &msg)
{
    strings_vec replies;
    (void)data;
    (void)client;
    (void)msg;
    showChan();
    return replies;
}
