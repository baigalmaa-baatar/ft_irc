#include "../incs/ircserv.h"

commands_map initialize_commands()
{
	commands_map commands;

	commands["CAP"] = &CAP;
	commands["PASS"] = &PASS;
	commands["NICK"] = &NICK;
	commands["USER"] = &USER;
	commands["OPER"] = &OPER;
	commands["KILL"] = &KILL;
	commands["PING"] = &PING;
	commands["PONG"] = &PONG;
	commands["QUIT"] = &QUIT;
	commands["JOIN"] = &JOIN;
	commands["PART"] = &PART;
	commands["MODE"] = &MODE;
	commands["MOTD"] = &MOTD;
	commands["KICK"] = &KICK;
	commands["NAMES"] = &NAMES;
	commands["TOPIC"] = &TOPIC;
	commands["INVITE"] = &INVITE;
	commands["NOTICE"] = &PRIVMSG;
	commands["PRIVMSG"] = &PRIVMSG;
	// commands["DEBUG"] = &DEBUG;

	return (commands);
}
