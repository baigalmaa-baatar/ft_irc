#include "../../incs/ircserv.h"

std::string ERR_NOSUCHNICK(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "401", "No such nick/channel"));
}

std::string ERR_NOORIGIN(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "409", "No origin specified"));
}

std::string ERR_UNKNOWNCOMMAND(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "421", "Unknown command"));
}

std::string ERR_NONICKNAMEGIVEN(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "431", "No nickname given"));
}

std::string ERR_ERRONEUSNICKNAME(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "432", "Erroneous nickname"));
}

std::string ERR_NICKNAMEINUSE(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "433", "Nickname is already in use"));
}

std::string ERR_NOTREGISTERED(t_data *data, Client *client, std::string info)
{
	// here info = command
	return (error_rpl(data, client, info, "451", "You have not registered"));
}

std::string ERR_NEEDMOREPARAMS(t_data *data, Client *client, std::string info)
{
	// here info = command
	return (error_rpl(data, client, info, "461", "Not enough parameters."));
}

std::string ERR_ALREADYREGISTERED(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "462", "You may not reregister"));
}

std::string ERR_PASSWDMISMATCH(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "464", "Password incorrect"));
}

std::string ERR_NOOPERHOST(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "491", "Invalid oper credentials"));
}

std::string ERR_BADCHANMASK(t_data *data, Client *client, std::string info)
{
	// std::string rpl = ":" + data->servername + " 476 " + spacing(client)
	// 				+ "PASS :Bad Channel Mask or Bad Channel Name\r\n";
	return (error_rpl(data, client, info, "476", "Bad Channel Mask or Bad Channel Name"));
}

std::string ERR_BADCHANNELKEY(t_data *data, Client *client, std::string info)
{
	// std::string rpl = ":" + data->servername + " 475 " + spacing(client)
	// 				+ <client> <channel> :Cannot join channel (+k)\r\n";
	return (error_rpl(data, client, info, "475", "Cannot join channel (+k)"));
}

std::string ERR_CHANNELISFULL(t_data *data, Client *client, std::string info)
{
	// std::string rpl = ":" + data->servername + " 471 " + spacing(client)
	// 				+ <client> <channel> :Cannot join channel (+k)\r\n";
	return (error_rpl(data, client, info, "471", "Cannot join channel (+l)"));
}

std::string ERR_USERONCHANNEL(t_data *data, Client *client, std::string info)
{
	//Returned when a client tries to invite <nick> to a channel they’re already joined to.
	return (error_rpl(data, client, info, "443", "is already on channel"));
}

std::string ERR_NOSUCHCHANNEL(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "403", "No such channel"));
}

std::string ERR_CHANOPRIVSNEEDED(t_data *data, Client *client, std::string info)
{
	//482
	/*Indicates that a command failed because the client
	does not have the appropriate channel privileges.
	This numeric can apply for different prefixes such as halfop, operator, etc.
	The text used in the last param of this message may vary.*/
	return (error_rpl(data, client, info, "403", "You're not channel operator"));
}

std::string ERR_INVALIDMODEPARAM(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "696", "Value is invalid"));
}

std::string ERR_UNKNOWNMODE(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "472", "is unknown mode char to me"));
}

std::string ERR_NOPRIVILEGES(t_data *data, Client *client, std::string info)
{
	return (error_rpl(data, client, info, "481", "Permission Denied - You're not an IRC operator"));
}
