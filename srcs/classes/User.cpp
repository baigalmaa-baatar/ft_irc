#include "../../incs/User.hpp"


/************* CONSTRUCTORS AND DESTRUCTOR *************/

User::User() : _nick(), _name(), _host(), _realName() {}

User::User(std::string ip) : _nick(), _name(), _host(ip), _realName() {}

User::~User() {}


/*********************** GETTERS ***********************/

const std::string & User::getNick() const {
	return (_nick);
}

const std::string & User::getName() const {
	return (_name);
}

const std::string & User::getHost() const {
	return (_host);
}

const std::string & User::getRealName() const {
	return (_realName);
}


/*********************** SETTERS ***********************/

void User::setNick(const std::string & nick) {
	_nick = nick;
}

void User::setName(const std::string & name) {
	_name = name;
}

void User::setHost(const std::string & host) {
	_host = host;
}

void User::setRealName(const std::string & realName) {
	_realName = realName;
}

