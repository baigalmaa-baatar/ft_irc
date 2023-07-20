#ifndef REGISTRATION_HPP
# define REGISTRATION_HPP

class Registration
{

	private :
	
		bool	_passOk;
		bool	_nickOk;
		bool	_userOk;

		bool	_registered;


	public :

		Registration();
		~Registration();
	
		void passCall();
		void nickCall();
		void userCall();

		bool isConnected() const;
		bool canRegister() const;
		void registerNow();
		bool isRegistered() const;

};

#endif