#ifndef SERVER_HPP
# define SERVER_HPP

#define		RED	"\033[31;1m"
#define		GRE	"\033[32;1m"
#define		YEL	"\033[33;1m"
#define		BLU	"\033[34;1m"
#define		NOC "\033[0m"

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h> //socket manage
#include <netdb.h> //socket manage
#include <cstring>
#include <unistd.h>
#include <iomanip>
//#include <sys/epoll.h>

#include <poll.h>
#include <string>


#include "to_str.hpp"
#include "define.hpp"
#include "ConfigFile.hpp"

#define DB_LISTENING "[SERVER_LISTENING]"

class ConfigFile;

class	Server 
{

	public:
	// The server might never be copied so no definition of server copy and server assignment
		Server(const char * port, const char *pass, ConfigFile *IRCconfig);
		~Server(void);
	// Server_stop.cpp
		void stop (void);			

		void test (void);

    private:
	// Server_getInfos.cpp
			// Server_getInfos.cpp
		void getInfos (void);
	// Server_closeFD.cpp
		void close_fd (int &fd, bool exception);
	// Server_listening.cpp
		void listening (void);
	// Server_monitoring.cpp
		void monitoring (void);

	//special debeug TEMP
	void ifModeDebug(const std::string&, const std::string&, int&);


		
		int							_port;
		std::string					_pass; //password server
		ConfigFile				*	_IRCconfig; //stock info config file
		std::string					_oper_pass; //no use for moment
		struct addrinfo			*	_addrs; //store info addr socket
		int 						_listener; // socket fd
		int 						_client_fd; // client fd
		int							_nb_ev;
		char 						_hostname[HOSTNAME_SIZE];
		int							_efd;
		char						_buffer[BUFFER_SIZE];
};
#endif
