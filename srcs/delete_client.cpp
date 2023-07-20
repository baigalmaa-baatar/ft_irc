#include "../incs/ircserv.h"

void deleteClientFromChans(Client *client)
{
	std::vector<std::string> others;
	std::string nick = client->getNick();
	for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end(); it++)
	{
		Client *client_ptr = (*it)->findUser(nick);
		// clean clients
		if (client_ptr)
		{
			std::string rpl = user_rpl(client_ptr, "QUIT", "Client exited");
			(*it)->sendAll(client_ptr, others, rpl, true);
			(*it)->removeUser(nick, false);
		}
	}

	// channel cleanup
	for (std::vector<Channel *>::iterator it = g_channels.begin(); it != g_channels.end();)
	{
		if ((*it)->getClients().empty())
		{
			delete *it;
			g_channels.erase(it);
		}
		else
			++it;
	}
}

void delete_client(t_data &data, int &i, int &nb_fds)
{
	std::cout << "[ " << YE << "connection to fd " << data.clients[i]->getFd() << " closed" << NC << " ]" << std::endl;

	// erasing client
	int save_fd;
	for (std::vector<Client *>::iterator it = data.clients.begin(); it != data.clients.end(); it++)
	{
		if ((*it)->getFd() == data.clients[i]->getFd())
		{
			save_fd = (*it)->getFd();
			deleteClientFromChans(*it);
			close(save_fd);
			delete (*it);
			data.clients.erase(it);
			break;
		}
	}

	// erasing pollfd from repertory
	for (std::vector<struct pollfd>::iterator it = data.pollfds.begin(); it != data.pollfds.end(); it++)
	{
		if (it->fd == save_fd)
		{
			data.pollfds.erase(it);
			break;
		}
	}

	i = 0;
	nb_fds--;
}

void delete_client(t_data &data, std::vector<Client *>::iterator &client_it, int &i, int &nb_fds)
{
	std::cout << "[ " << YE << "connection to fd " << (*client_it)->getFd() << " closed" << NC << " ]" << std::endl;

	// erasing client
	int save_fd = (*client_it)->getFd();
	close(save_fd);
	delete (*client_it);
	data.clients.erase(client_it);

	// erasing pollfd from repertory
	for (std::vector<struct pollfd>::iterator poll_it = data.pollfds.begin(); poll_it != data.pollfds.end(); poll_it++)
	{
		if (poll_it->fd == save_fd)
		{
			data.pollfds.erase(poll_it);
			break;
		}
	}

	i = 0;
	nb_fds--;
}
