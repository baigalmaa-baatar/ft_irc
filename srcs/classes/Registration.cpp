#include "../../incs/Registration.hpp"


/************* CONSTRUCTORS AND DESTRUCTOR *************/

Registration::Registration() : _passOk(false), _nickOk(false), _userOk(false) , _registered(false) {}

Registration::~Registration() {}


/*********************** GETTERS ***********************/


/*********************** SETTERS ***********************/


/******************* OTHER FUNCTIONS *******************/

void Registration::passCall() {
	if (!_nickOk && !_userOk)
		_passOk = true;
}

void Registration::nickCall() {
	_nickOk = true;
}

void Registration::userCall() {
	_userOk = true;
}

bool Registration::isConnected() const {
	return (_passOk);
}

bool Registration::canRegister() const {
	return (_passOk && _nickOk && _userOk);
}

void Registration::registerNow() {
	_registered = true;
}

bool Registration::isRegistered() const {
	return (_registered);
}

