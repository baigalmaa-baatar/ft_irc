#include "../../incs/ircserv.h"

void send_replies(strings_vec replies, Client *client)
{
	strings_vec::iterator reply_it = replies.begin();
	strings_vec::iterator reply_end = replies.end();
	for (; reply_it != reply_end; reply_it++) // iterate on each reply in order to send it back to the client
	{
		if (reply_it->empty())
			continue;

		std::cout << BC << "replied = " << CY << *reply_it << NC;
		int res = send(client->getFd(), reply_it->c_str(), reply_it->size(), 0);
		if (res == -1)
			std::cout << "Problem when sending the reply back to the client" << std::endl;
	}
}
