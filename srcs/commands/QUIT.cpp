#include "../../incs/ircserv.h"

std::string format(std::string s)
{
	std::string result;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = (i && s[i] >= 'A' && s[i] <= 'Z' ? s[i] + 32 : s[i]);
		result.push_back(c);
	}
	return (result);
}

strings_vec QUIT(t_data *data, Client *client, t_msg &msg)
{
	//warn other clients that this client is leaving and delete him from all channels
	deleteClientFromChans(client);

	t_msg reason;
	reason.command = format(msg.command);
	if (msg.param.empty())
		reason.param = "leaving";
	else
	{
		for (std::size_t i = 0; i < msg.params.size(); i++)
			reason.param += (i != msg.params.size() - 1 ? (msg.params[i] + " ") : msg.params[i]);
	}
	return (ERROR(data, client, reason));
}
