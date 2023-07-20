#include "../incs/ircserv.h"

int error_exit(std::string msg, bool use_perror, int socket_fd, t_data *data)
{
	std::string err_msg = RE + msg + NC;
	if (!data->clients.empty())
	{
		std::vector<Client *>::reverse_iterator it_end = data->clients.rend() - 1;
		for (std::vector<Client *>::reverse_iterator it = data->clients.rbegin(); it != it_end; it++)
		{
			deleteClientFromChans(*it);
			close((*it)->getFd());
			delete (*it);
		}
		data->clients.clear();
		data->pollfds.clear();
	}
	if (socket_fd != -1)
		close(socket_fd);
	if (use_perror)
		perror(err_msg.c_str());
	else
		std::cerr << err_msg << std::endl;
	return (-1);
}