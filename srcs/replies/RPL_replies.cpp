#include "../../incs/ircserv.h"

std::string RPL_WELCOME(t_data *data, Client *client)
{
	std::string msg = "Welcome to the Internet Relay Network " + client->getFullName();
	std::string rpl = regular_rpl(data, client, "001", msg);
	return (rpl);
}

std::string RPL_YOURHOST(t_data *data, Client *client)
{
	std::string msg = "Your host is " + data->servername + ", running version 1.0";
	std::string rpl = regular_rpl(data, client, "002", msg);
	return (rpl);
}

std::string RPL_CREATED(t_data *data, Client *client)
{
	return (regular_rpl(data, client, "003", "This server was created 14 Dec 2022"));
}

std::string RPL_MYINFO(t_data *data, Client *client)
{
	std::string	rpl = ":" + data->servername + " 004 " + client->getNick() \
					+ " " + data->servername + " 1 io klmov " + "\r\n";
	return (rpl); 
}

std::string RPL_YOUREOPER(t_data *data, Client *client)
{
	return (regular_rpl(data, client, "381", "You are now an IRC operator"));
}

std::string RPL_INVITING(t_data *data, Client *client, std::string info)
{
	return (regular_rpl(data, client, "341", info));
}

std::string RPL_CHANNELMODEIS(t_data *data, Client * client, std::string line)
{
//   "<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"
// Part of the post-registration greeting. Clients SHOULD discover available features
// using RPL_ISUPPORT tokens rather than the mode letters listed in this reply.
     // :bbaatar3!~bbaatar3@freenode-1fl.s40.6vib9m.IP MODE #chan1 +k :pass1

	std::string	rpl = client->getNick() + "!~" + client->getNick() + data->servername  \
					// + channe->getChanName() 

					+ line  + "\r\n";
	return (rpl);

	//324
	/*Sent to a client to inform them of the currently-set
	modes of a channel. <channel> is the name of the channel.
	 <modestring> and <mode arguments> are a mode string and
	 the mode arguments (delimited as separate parameters)
	as defined in the MODE message description.*/
	(void)data;
	(void)client;
	(void)line;
	return ("MODE:Channel mode has set to \r\n"); // need to fix later

}

std::string RPL_MOTD(t_data *data, Client *client)
{
	std::string rpl;
	rpl += regular_rpl(data, client, "372", "                                         ");
	rpl += regular_rpl(data, client, "372", "   · · · · · welcome · to ·· our · · · · ·  ");
	rpl += regular_rpl(data, client, "372", " ·                                         ·");
	rpl += regular_rpl(data, client, "372", " ·  (_)                                    ·");
	rpl += regular_rpl(data, client, "372", " ·   _  ____ ____  ___  ____  ____ _   _   ·");
	rpl += regular_rpl(data, client, "372", " ·  | |/ ___) ___)/___)/ _  )/ ___) | | |  ·");
	rpl += regular_rpl(data, client, "372", " ·  | | |  ( (___|___ ( (/ /| |    \\ V /   ·");
	rpl += regular_rpl(data, client, "372", " ·  |_|_|   \\____|___/ \\____)_|     \\_/    ·");
	rpl += regular_rpl(data, client, "372", " ·                                         ·");
	rpl += regular_rpl(data, client, "372", "   · · · · · · · · · · · · · · · · · · · ·  ");
	rpl += regular_rpl(data, client, "372", "                                          ");
	return (rpl); 
}

std::string RPL_MOTDSTART(t_data *data, Client *client)
{
	return (regular_rpl(data, client, "375", ("- " + data->servername + " message of the day :) -"))); 
}

std::string RPL_ENDOFMOTD(t_data *data, Client *client)
{
	return (regular_rpl(data, client, "376", ("- end of message of the day ! -"))); 
}
