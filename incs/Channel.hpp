#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include <string>
#include <vector>
#include "ircserv.h"

class Client;

class Channel
{
	private :

		std::string 			_chan_name;
		std::string 			_topic;
		std::string 			_mode;
		std::vector<Client *>	_clients;
		std::string 			_key;
		std::vector<Client *>	_voiced;
		std::vector<Client *>	_invited;

	public:
		std::vector<Client *>	_operator;
		size_t					chan_user_limit;
		bool					flag_key;
		bool					flag_channel_user_limit;
		bool					flag_invite_only;
		bool					flag_moderated;

	public:

		Channel();
		Channel(const std::string chan_name);

		~Channel();

		Channel const & operator=(const Channel & copy);

		const std::string & 				getChanName() const ;
		const std::string & 				getTopic() const ;
		const std::string & 				getMode() const ;
		std::vector<Client *> &				getClients() ;
		const std::string & 				getKey() const ;

		void			setChanName(const std::string & name) ;
		void			setMode(const std::string & mode) ;
		void			setTopic(const std::string & topic) ;
		void			setChannelKey(const std::string & key) ;
		void			setChannelLimit(const size_t limit) ;
		void 			setChannelInviteOnly(void) ;
		void 			setChannelModerated(void) ;
		void			sendChanOps(std::string sender, std::string msg);
		void 			sendAll(std::string sender, std::string msg, bool ignore_self);
		void			sendAll(Client *client, std::vector<std::string> &others, std::string rpl, bool ignore_self);



	//Member functions:
	public:
		bool 				rename(std::string new_chan_name);
        void				addClientToChannel(Client * client);
		void				removeUser(std::string nick, bool notify);
		bool				addOperator(Client *client, Client *user);
		bool				removeOperator(Client *client, Client *user, bool notify);
		bool				isOperator(Client *user);
		bool				addVoiced(Client *client, Client *user);
		bool				removeVoiced(Client *client, Client *user, bool notify);
		bool				isVoiced(Client *user);
		bool				addInvited(Client *user);
		bool				removeInvited(Client *user);
		bool				isInvited(Client *user);
		void 				removeChannelKey(void);
		void 				removeChannelUserLimit(void);
		static Channel * 	findChannel(std::string chan_name);
		Client *			findUser(std::string nick);
		void 				removeChannelInviteOnly(void) ;
		void 				removeChannelModerated(void) ;

	friend class Client;
};

#endif
