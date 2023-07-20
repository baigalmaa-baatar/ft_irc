#include "../../incs/ircserv.h"

std::string spacing(const std::string &s)
{
	std::string format = s.empty() ? "" : (s + " ");
	return (format);
}


std::string regular_rpl(t_data *data, Client *client, std::string rpl_id, std::string rpl_msg)
{
	/* 
	  structure:
 		1) source (:server)
		2) rpl id
		3) client.nick (* if nick not defined yet ?)
		3) msg
 	*/
	std::string rpl = ":" + data->servername + " " + rpl_id + " " \
					+ (client && !client->getNick().empty() ? spacing(client->getNick()) : "* ") \
					+ ":" + rpl_msg + "\r\n";
	return (rpl);
}

std::string error_rpl(t_data *data, Client *client, std::string info, std::string err_id, std::string err_msg)
{
	/* 
	  structure:
 		1) source (:server)
		2) rpl id
		3) client.nick (* if nick not defined yet)
 		4) cmd/info (optionnal)
		3) msg
 	*/
	std::string rpl = ":" + data->servername + " " + err_id + " " \
					+ (client && !client->getNick().empty() ? spacing(client->getNick()) : "* ") \
					// + (channel ? spacing(channel->getChanName()) : "") //("MODE " + params[0])
					+ spacing(info) \
					+ (err_msg.empty() ? "" : (":" + err_msg)) \
					+ "\r\n";
	return (rpl);
}

std::string user_rpl(Client *client, std::string cmd, std::string params)
{
	/* 
	  structure: (for now)
 		1) source (:nick!user@host)
 		2) cmd
		3) params
 	*/
	std::string rpl = ":" + client->getNick() \
					+ "!" + client->getName() \
					+ "@" + client->getHost() \
					+ " " + cmd \
					+ " :" + params \
					+ "\r\n";
	return (rpl);
}
