#include "../incs/ircserv.h"

std::vector<Channel *> g_channels;

int main(int argc, char *argv[])
{
	t_data data;

	if (initialize_data(argc, argv, &data) == -1)
		return (-1);

	// create socket
	int socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	int option = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	if (socket_fd == -1)
		return (error_exit("/!\\ Socket", 1, socket_fd, &data));
	std::cout << "[ server socket succesfully created ]" << std::endl;

	// set info
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(data.port);
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	// bind
	if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		return (error_exit("/!\\ Bind", 1, socket_fd, &data));
	std::cout << "[ binding succesful ]" << std::endl;

	// listen
	listen(socket_fd, SOMAXCONN);
	std::cout << "[ listening... ]" << std::endl;


	// start multiplexing
	int nb_fds = 1;
	int received = 0;

	Client server(server_address, socket_fd);
	data.clients.push_back(&server);

	// starting by adding one pollfd struct to pollfds. it's the socket fd
	struct pollfd s_poll;
	memset(&s_poll, 0, sizeof(s_poll));
	s_poll.fd = socket_fd;
	s_poll.events = POLLIN;
	data.pollfds.push_back(s_poll);

	while (true)
	{
		// first: poll with pollfds pointer, number of fds and timeout null
		int ret = poll(data.pollfds.data(), nb_fds, 0);
		if (ret == -1)
		{
			perror("poll");
			break ;
		}
		else // then we're gonna check all of the fds
		{
			for (int i = 0; i < nb_fds; i++)
			{
				if (!data.pollfds[i].revents) // first we check if the fd received something, if not, we go directly to the next fd
					continue ;
				if (data.pollfds[i].fd == socket_fd) // then we check if we're dealing with the socket fd (server side). here we're gonna accept new clients
				{
					bool add = true;
					while (add)
        			{
						Client *new_client = new Client;
						new_client->setFd(accept(socket_fd, (struct sockaddr *)new_client->getSockAddress(), new_client->getSizeAddress()));
						if (new_client->getFd() > 0) // if accept() call succesful
						{
							std::cout << "[ new client connection on " << YE << "fd " << new_client->getFd() << NC << " ]" << std::endl;

							char tmp_buf[100] = {0};
							char *x = tmp_buf;
							x = inet_ntoa(new_client->getSock().sin_addr);
							new_client->setIp(std::string(x));
							data.clients.push_back(new_client); //adding new client to repertory

							struct pollfd new_s_poll; // also adding a new pollfd with the info of the new client
							memset(&new_s_poll, 0, sizeof(new_s_poll));
							new_s_poll.fd = new_client->getFd();
							new_s_poll.events = POLLIN;
							data.pollfds.push_back(new_s_poll);

							nb_fds++;
						}
						else
						{
							add = false;
							delete (new_client);
						}
        			}
	  			}
				else // then, if it's another fd (client side), we can process the incoming information
				{
					std::cout << "[ " << YE << "in fd " << data.pollfds[i].fd << NC << " ]" << std::endl;

					Client *client = getClient(&data, data.pollfds[i].fd);
					if (!client)
						continue;					
					if ((received = recv(client->getFd(), client->fillBuf(), 512, MSG_DONTWAIT)) == -1)
						continue;
					else if (!received) // when recv returns 0, that means the connection was closed. (ex : Ctrl+C with netcat)
						delete_client(data, i, nb_fds);
					else if (!ends(client->fillBuf()))
						client->updateBuf();
					else
					{
						parsing(&data, client, std::string(client->updateBuf()));							
						client->clearBuf();
	
						//check if we have to close some clients
						std::vector<Client*>::iterator it_end = data.clients.end();
						for (std::vector<Client*>::iterator it = data.clients.begin(); it != it_end; it++)
						{
							if ((*it)->mustClose())
								delete_client(data, it, i, nb_fds);
						}
					}
				}
			}
		}
	}
	return (0);
}
