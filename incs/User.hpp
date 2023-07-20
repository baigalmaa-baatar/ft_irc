#ifndef USER_HPP
# define USER_HPP

#include <string>

class Client;

class User
{
	private :

		std::string _nick;
		std::string _name;
		std::string _host;
		std::string _realName;


	public :

		User();
		User(std::string ip);
		~User();

		const std::string & getNick() const;
		const std::string & getName() const;
		const std::string & getHost() const;
		const std::string & getRealName() const;
		
		void setNick(const std::string & nick);
		void setName(const std::string & name);
		void setHost(const std::string & host);
		void setRealName(const std::string & realName);

	friend class Client;
};

#endif