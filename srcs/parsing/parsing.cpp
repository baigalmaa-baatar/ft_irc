#include "../../incs/ircserv.h"

void checkRegistration(t_data *data, Client *client, strings_vec &replies)
{
	if (client->isConnected())
	{
		if (!client->isRegistered() && client->canRegister())
		{
			replies.push_back(RPL_WELCOME(data, client));
			replies.push_back(RPL_YOURHOST(data, client));
			replies.push_back(RPL_CREATED(data, client));
			replies.push_back(RPL_MYINFO(data, client));
			client->registerNow();
			
			t_msg msg;
			strings_vec motd = MOTD(data, client, msg);
			replies.insert(replies.end(), motd.begin(), motd.end());
		}
	}
}

void parsing(t_data *data, Client *client, std::string buf)
{
	strings_vec received = separate_lines(buf); // get lines sent by the client
	strings_vec::iterator received_it = received.begin();
	strings_vec::iterator received_end = received.end();

	strings_vec replies; // a vector of replies to store the replies of the command functions

	for (; received_it != received_end; received_it++) // iterate on each line received in order to analyze it
	{
		std::cout << BG << "received = " << GN << *received_it << NC << std::endl; //tmp

		t_msg msg = create_msg(*received_it); //parse received line

		commands_map::iterator commands_it = data->commands.find(msg.command); //then look for the corresponding command
		if (commands_it != data->commands.end())
		{
			strings_vec result = commands_it->second(data, client, msg);
			replies.insert(replies.end(), result.begin(), result.end());
			checkRegistration(data, client, replies);
		}
		else if (!msg.command.empty())
			replies.push_back(ERR_UNKNOWNCOMMAND(data, client, msg.command));
	}
	send_replies(replies, client);
}
