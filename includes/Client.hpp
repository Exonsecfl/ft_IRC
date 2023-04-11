#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "define.hpp"
# include <iostream>
# include <string>

class Client
{
private:

	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _password;
	std::string _modes;
	std::string _canal;
	int			_clientFd;

public:



	void setNickname(std::string newNick);
	std::string getNickname();

	int
	Client(int client_fd, std::string client_data);
	~Client();
};

#endif
