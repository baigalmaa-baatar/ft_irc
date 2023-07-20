#include "../incs/ircserv.h"

int	ft_atoi(const char *str)
{
	int	i = 0;
	int	nbr = 0;
	int	neg = 1;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg = -1;
		i++;
	}
	while (str[i] == '0')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = (nbr * 10) + str[i] - 48;
		i++;
	}
	return (nbr * neg);
}

int initialize_data(int argc, char *argv[], t_data *data)
{
	if (argc != 3)
		return (error_exit("Error: Wrong number of arguments\nUsage: \"./ircserv <port> <password>\"", 0, -1, data));

	int port = ft_atoi(argv[1]);
	if (port < 1 || port > 65536)
		return (error_exit("Error: Invalid port number", 0, -1, data));
	data->port = port;
	data->password = std::string(argv[2]);
	data->servername = "ircserv";
	data->oper[0] = "operator"; /* this is the operator id */
	data->oper[1] = "secret"; /* this is the operator pwd */
	data->commands = initialize_commands();
	return (1);
}