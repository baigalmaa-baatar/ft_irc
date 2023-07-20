#ifndef CLIENT_HPP
# define CLIENT_HPP


#include "ircserv.h"

class Buffer;
class Registration;

class Client
{

	private :

		struct sockaddr_in	_sock;
		socklen_t			_size;
		int					_fd;
		std::string			_ip;
		User				_user;
		Buffer				_buf;
		Registration		_reg;
		bool				_mustClose;

	public :

		bool					flag_oper; // user mode : operator
		bool					flag_invisible; //user mode +i invisible



	public :

		Client();
		Client(struct sockaddr_in &sock, int &fd);
		~Client();

		/* value getters */
		const struct sockaddr_in &getSock() const;
		const socklen_t &getSize() const;
		const int &getFd() const;
		const std::string &getNick() const;
		const std::string &getName() const;
		const std::string &getHost() const;
		const std::string &getRealName() const;
		const std::string getFullName() const;
		bool getOperatorMode();

		/* address getters */
		struct sockaddr_in *getSockAddress();
		socklen_t *getSizeAddress();
		char *getBuf();
		char *fillBuf();
		char *updateBuf();

		/* setters */
		void setFd(const int &fd);
		void setIp(const std::string &ip);
		void setNick(const std::string &nick);
		void setName(const std::string &name);
		void setHost(const std::string &host);
		void setRealName(const std::string &realName);
		void setInvisibleMode(void);
		void setOperatorMode(void);

		/* buffer */
		void clearBuf();

		/* registration */
		void passCall();
		void nickCall();
		void userCall();
		bool isConnected() const;
		bool canRegister() const;
		void registerNow();
		bool isRegistered() const;

		/* closure */
		void notifClose();
		bool mustClose() const;

		/* modes */
		void removeInvisibleMode(void);
		void removeOperatorMode(void);

};

#endif
