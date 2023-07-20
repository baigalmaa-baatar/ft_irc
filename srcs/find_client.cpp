#include "../incs/ircserv.h"

Client * findClient(t_data * data, std::string nick)
{
    for (size_t i = 1; i < data->clients.size(); i++) {
        if (!data->clients[i]->getNick().empty() && nick == data->clients[i]->getNick()) {
            return data->clients[i];
        }
    }

    return NULL;
}
