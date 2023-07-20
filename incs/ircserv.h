#ifndef IRCSERV_H
# define IRCSERV_H

# include <string>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdio.h>
# include <poll.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <map>
# include <vector>
# include <algorithm>
# include <sstream>
# include <netdb.h>
# include <arpa/inet.h>
# include <fnmatch.h>
# include <string.h>

# include "User.hpp"
# include "Buffer.hpp"
# include "Registration.hpp"
# include "Client.hpp"
# include "Channel.hpp"

extern std::vector<Channel *> g_channels;


// TYPEDEFS
typedef	struct s_msg														t_msg;
typedef	struct s_data														t_data;
typedef	std::vector<std::string>											strings_vec;
typedef	std::map<std::string, strings_vec (*)(t_data *, Client*, t_msg&)>	commands_map;

typedef struct s_data
{
	int 						port;
	std::string 				password;
	std::string 				servername;
	std::string					oper[2];
	commands_map				commands;
	std::vector<Client*>		clients;
	std::vector<struct pollfd>	pollfds;
}								t_data;

typedef struct s_msg // structure of an IRC msg : ['@' <tags> SPACE] [':' <source> SPACE] <command> <parameters> <crlf>
{
	std::string raw; // raw message, will be used for forwarding to other users
	std::string	tags; // optional, = <tag> [';' <tag>]*
	std::string	source; // optional, = servername / ( nickname [ "!" user ] [ "@" host ] )
	std::string	command; // letter* or 3digit
	std::string	param; // = *( SPACE middle ) [ SPACE ":" trailing ];
	strings_vec	params; // = *( SPACE middle ) [ SPACE ":" trailing ];

	s_msg() : raw(), tags(), source(), command(), param(), params() {};
}				t_msg;


// GENERAL
int				initialize_data(int argc, char *argv[], t_data *data);
int				error_exit(std::string msg, bool use_perror, int socket_fd, t_data *data);
void			delete_client(t_data &data, int &i, int &nb_fds);
void			delete_client(t_data &data, std::vector<Client*>::iterator &client_it, int &i, int &nb_fds);
void			deleteClientFromChans(Client *client);
Client			*getClient(t_data *data, int fd);


// PARSING
void			parsing(t_data *data, Client *client, std::string buf);
t_msg			create_msg(std::string line_received);
std::string		parse_command(std::string &command);
strings_vec		parse_params(std::string &params);
bool			matchWildcard(const std::string &name, const std::string &pattern);
std::string		matchNames(t_data *data, const std::string &input);


// COMMANDS
commands_map	initialize_commands();
strings_vec		CAP(t_data *data, Client *client, t_msg &msg);
strings_vec		PASS(t_data *data, Client *client, t_msg &msg);
strings_vec 	NICK(t_data *data, Client *client, t_msg &msg);
strings_vec 	USER(t_data *data, Client *client, t_msg &msg);
strings_vec		MOTD(t_data *data, Client *client, t_msg &msg);
strings_vec 	OPER(t_data *data, Client *client, t_msg &msg);
strings_vec		KILL(t_data *data, Client *client, t_msg &msg);
strings_vec		PING(t_data *data, Client *client, t_msg &msg);
strings_vec		PING();
strings_vec 	PONG(t_data *data, Client *client, t_msg &msg);
strings_vec		PONG(t_data * data, t_msg &msg);
strings_vec 	QUIT(t_data *data, Client *client, t_msg &msg);
strings_vec 	ERROR(t_data *data, Client *client, t_msg &msg);
strings_vec 	JOIN(t_data *data, Client *client, t_msg &msg);
strings_vec 	PART(t_data *data, Client *client, t_msg &msg);
strings_vec 	MODE(t_data *data, Client *client, t_msg &msg);
strings_vec		PRIVMSG(t_data *data, Client *client, t_msg &msg);
strings_vec		KICK(t_data *data, Client *client, t_msg &msg);
strings_vec		INVITE(t_data *data, Client *client, t_msg &msg);
strings_vec		TOPIC(t_data *data, Client *client, t_msg &msg);
strings_vec		GET_TOPIC(t_data *data, Client *client, std::string channelName);
strings_vec		NAMES(t_data *data, Client *client, t_msg &msg);
strings_vec		NAMES(t_data *data, Client *client, std::string channelName);
strings_vec		NAMES(t_data *data, Client *client, std::string channelName);
strings_vec		DEBUG(t_data *data, Client *client, t_msg &msg);


// REPLIES
void			send_replies(strings_vec replies, Client *client);
std::string		regular_rpl(t_data *data, Client *client, std::string rpl_id, std::string rpl_msg);
std::string		error_rpl(t_data *data, Client *client, std::string info, std::string err_id, std::string err_msg);
std::string 	user_rpl(Client *client,std::string cmd, std::string params);
std::string		spacing(const std::string & s);

Client			*findClient(t_data *data, std::string nick);
bool			in_list(std::vector<Client *> list, Client *client);
bool			add_into_list(std::vector<Client *> &list, Client *client);
bool			remove_from_list(std::vector<Client *> &list, Client *client);

/* regular replies */
std::string		RPL_WELCOME(t_data *data, Client *client);
std::string		RPL_YOURHOST(t_data *data, Client *client);
std::string		RPL_CREATED(t_data *data, Client *client);
std::string		RPL_MYINFO(t_data *data, Client *client);
std::string		RPL_YOUREOPER(t_data *data, Client *client);
std::string		RPL_CHANNELMODEIS(t_data *data, Client *client, std::string info = "");
std::string		RPL_MOTD(t_data *data, Client *client);
std::string		RPL_MOTDSTART(t_data *data, Client *client);
std::string		RPL_ENDOFMOTD(t_data *data, Client *client);
std::string		RPL_INVITING(t_data *data, Client *client, std::string info = "");

/* error replies */
std::string		ERR_NOSUCHNICK(t_data *data, Client *client, std::string info = "");
std::string		ERR_NOORIGIN(t_data *data, Client *client, std::string info = "");
std::string 	ERR_NONICKNAMEGIVEN	(t_data *data, Client *client, std::string info = "");
std::string 	ERR_ERRONEUSNICKNAME(t_data *data, Client *client, std::string info = "");
std::string 	ERR_NICKNAMEINUSE(t_data *data, Client *client, std::string info = "");
std::string		ERR_NEEDMOREPARAMS(t_data *data, Client *client, std::string info = "");
std::string		ERR_NOTREGISTERED(t_data *data, Client *client, std::string info);
std::string		ERR_ALREADYREGISTERED(t_data *data, Client *client, std::string info = "");
std::string		ERR_PASSWDMISMATCH(t_data *data, Client *client, std::string info = "");
std::string		ERR_NOOPERHOST(t_data *data, Client *client, std::string info = "");
std::string		ERR_UNKNOWNCOMMAND(t_data *data, Client *client, std::string info = "");
std::string		ERR_NOSUCHCHANNEL(t_data *data, Client *client, std::string info = "");
std::string		ERR_CHANOPRIVSNEEDED(t_data *data, Client *client, std::string info = "");
std::string		ERR_INVALIDMODEPARAM(t_data *data, Client *client, std::string info = "");
std::string 	ERR_BADCHANMASK		(t_data *data, Client *client, std::string info = "");
std::string 	ERR_BADCHANNELKEY	(t_data *data, Client *client, std::string info = "");
std::string 	ERR_CHANNELISFULL	(t_data *data, Client *client, std::string info = "");
std::string 	ERR_USERONCHANNEL	(t_data *data, Client *client, std::string info = "");
std::string		ERR_NOPRIVILEGES(t_data *data, Client *client, std::string info = "");


// UTILS
strings_vec 	split(std::string s, std::string sep);
strings_vec 	separate_words(std::string s);
strings_vec 	separate_lines(std::string s);
bool			contains(const char *chars, std::string s);
bool			starts(const char *chars, std::string s);
bool			ends(char *buffer);


// COLORS

/* regular */
#define RE "\e[0;31m"
#define GN "\e[0;32m"
#define YE "\e[0;33m"
#define OR "\e[0;34m"
#define MA "\e[0;35m"
#define CY "\e[0;36m"
#define NC "\e[0m"

/* bold */
#define BN "\e[1;30m"
#define BR "\e[1;31m"
#define BG "\e[1;32m"
#define BY "\e[1;33m"
#define BO "\e[1;34m"
#define BP "\e[1;35m"
#define BC "\e[1;36m"
#define BW "\e[1;37m"

/* underlined */
#define UN "\e[4;30m"
#define UR "\e[4;31m"
#define UG "\e[4;32m"
#define UY "\e[4;33m"
#define UO "\e[4;34m"
#define UP "\e[4;35m"
#define UC "\e[4;36m"
#define UW "\e[4;37m"

#endif
