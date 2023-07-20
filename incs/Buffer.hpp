#ifndef BUFFER_HPP
# define BUFFER_HPP

#include <string>

# define BUF_SIZE 512

class Buffer
{

	private :

		char	_buf1[BUF_SIZE];
		char	_buf2[BUF_SIZE];


	public :

		Buffer();
		~Buffer();

		char * getBuf(int id);
		
		void clearBuf(int id);
		void clearBufs();
		void join();

};

#endif