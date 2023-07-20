#include "../../incs/ircserv.h"

std::string get_params(std::string &line_received, std::string &save_cmd)
{
	std::size_t pos = line_received.find(save_cmd);
	if (pos == std::string::npos)
		return (line_received);
	if (pos + save_cmd.size() + 1 < line_received.size())
		return (line_received.substr(pos + save_cmd.size() + 1));
	return (line_received.substr(pos + save_cmd.size()));
}

t_msg create_msg(std::string line_received)
{
	t_msg msg;
	msg.raw = line_received;
	strings_vec	split = separate_words(line_received);
	std::string save_cmd;
	bool tags_ok = false;
	bool source_ok = false;
	bool command_ok = false;
	bool params_ok = false;
	for (std::size_t i = 0; i < split.size(); i++)
	{
		if (!split.empty() && !split[i].empty())
		{
			if (split[i][0] == '@' && !tags_ok && !source_ok && !command_ok && !params_ok)
			{
				msg.tags = split[i];
				tags_ok = true;
			}
			else if (split[i][0] == ':' && !source_ok && !command_ok && !params_ok)
			{
				msg.source = split[i];
				source_ok = true;
			}
			else if (!command_ok && !params_ok)
			{
				msg.command = parse_command(split[i]);
				save_cmd = split[i];
				command_ok = true;
			}
			else if (!params_ok)
			{
				msg.param = get_params(line_received, save_cmd);
				params_ok = true;
				break ;
			}
		}
	}	
	msg.params = parse_params(msg.param);
	// std::cout << BR << "msg.tags = " << msg.tags << std::endl \
	// 	<< "msg.source = " << msg.source << std::endl \
	// 	<< "msg.command = " << msg.command << std::endl \
	// 	<< "msg.params = " << msg.param << NC << std::endl;
	// for (strings_vec::iterator it = msg.params.begin(); it != msg.params.end(); it++)
	// 	std::cout << UR << "parsed.params[] = " << *it << NC << std::endl;
	return (msg);
}
