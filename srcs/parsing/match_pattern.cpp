#include "../../incs/ircserv.h"

bool matchWildcard(const std::string &name, const std::string &pattern)
{
    return(fnmatch(pattern.c_str(), name.c_str(), FNM_NOESCAPE) == 0);
}

std::string matchNames(t_data *data, const std::string &input)
{
    size_t pos = input.find("!");
    size_t pos2 = input.find("@");
    if (pos != std::string::npos)
    {
        // full name -> dan!daniel.kaluuya@127.0.0.1
        for (size_t i = 1; i < data->clients.size(); i++)
        {
            //compare full name
            if (data->clients[i]->getFullName() == input)
                return(data->clients[i]->getNick());
        }
    }
    
    else if (pos2 != std::string::npos)
    {
        //user name + host name -> daniel.kaluuya@127.0.0.1
        for (size_t i = 1; i < data->clients.size(); i++)
        {
            //compare user name
            if (data->clients[i]->getName() == input.substr(0, pos2))
                return(data->clients[i]->getNick());
        }
    }

    else 
    {
        // only nick name
        return (input);
    }
    return ("");
}
