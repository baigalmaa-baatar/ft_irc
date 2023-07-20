#include "../../incs/Client.hpp"

/************* CONSTRUCTORS AND DESTRUCTOR *************/

Client::Client() : _sock(), _size(sizeof(_sock)), _fd(-1), _ip(), _user(), _buf(), _reg(), _mustClose(false), flag_oper(false), flag_invisible(false) {
	_buf.clearBufs();
}

//special constructor for socket fd (server)
Client::Client(struct sockaddr_in &sock, int &fd) : _sock(sock), _size(sizeof(_sock)), _fd(fd), _ip(), _user(_ip), _mustClose(false), flag_oper(false), flag_invisible(false) {}

Client::~Client() {}

/*********************** GETTERS ***********************/

const struct sockaddr_in &Client::getSock() const {
	return (_sock);
}

const socklen_t &Client::getSize() const {
	return (_size);
}

const int &Client::getFd() const {
	return (_fd);
}

const std::string &Client::getNick() const {
	return (_user._nick);
}

const std::string &Client::getName() const {
	return (_user._name);
}

const std::string &Client::getHost() const {
	return (_user._host);
}

const std::string &Client::getRealName() const {
	return (_user._realName);
}

const std::string Client::getFullName() const
{
	return (_user._nick + "!" + _user._name + "@" + _user._host);
}

bool Client::getOperatorMode()
{
	return flag_oper;
}

struct sockaddr_in *Client::getSockAddress() {
	return (&_sock);
}

socklen_t *Client::getSizeAddress() {
	return (&_size);
}

char *Client::getBuf() {
	return (_buf.getBuf(0));
}

char *Client::fillBuf() {
	return (_buf.getBuf(2));
}

char *Client::updateBuf() {
	return (_buf.getBuf(1));
}


/*********************** SETTERS ***********************/

void Client::setFd(const int &fd) {
	_fd = fd;
}

void Client::setIp(const std::string & ip) {
	_ip = ip;
	_user.setHost(_ip);
}

void Client::setNick(const std::string &nick) {
	_user._nick = nick;
}

void Client::setName(const std::string &name) {
	_user._name = name;
}

void Client::setHost(const std::string &host) {
	_user._host = host;
}

void Client::setRealName(const std::string & realName) {
	_user._realName = realName;
}

void Client::setInvisibleMode(void)
{
	flag_invisible = true;
}

void Client::setOperatorMode(void) {
	flag_oper = true;
}

/******************* OTHER FUNCTIONS *******************/

void Client::clearBuf() {
	_buf.clearBufs();
}

void Client::passCall() {
	_reg.passCall();
}

void Client::nickCall() {
	_reg.nickCall();
}

void Client::userCall() {
	_reg.userCall();
}

bool Client::isConnected() const {
	return (_reg.isConnected());
}

bool Client::canRegister() const {
	return (_reg.canRegister());
}

void Client::registerNow() {
	_reg.registerNow();
}

bool Client::isRegistered() const {
	return (_reg.isRegistered());
}

void Client::notifClose() {
	_mustClose = true;
}

bool Client::mustClose() const {
	return (_mustClose);
}

void Client::removeInvisibleMode(void)
{
	flag_invisible = false;
}

void Client::removeOperatorMode(void) {
	flag_oper = false;
}

