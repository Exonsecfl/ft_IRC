#include "Server.hpp"

void	Server::close_fd(int &fd, bool exception)
{
	if (fd >= 0)
	{
		if (0 > close(fd))
		{
			std::stringstream ss;
			ss << "[SERVER_CLOSEFD] ERROR - Failed to close fd nb[" << fd << "]";
			fd = -1;
			if (exception)
				throw std::runtime_error(ss.str());
		}
		for (std::vector< struct pollfd >::iterator it = (this->_fds).begin(); it != (this->_fds).end(); ++it)
		{
			if ( it->fd == fd)
			{
				// this->_fds.erase(it);
				break;
			}
		}
		this->_clientList.erase(_fd_nick_list[fd]);
		this->_fd_nick_list.erase(fd);
		std::stringstream ss;
		ss << "[SERVER_CLOSEFD] - Succesfully closed fd nb[" << fd << "]";
		fd = -1;
	}
	else
		std::cout << ("[SERVER_CLOSEFD] - WARN - try to close fd -1");
}

// struct pollfd {
// 	int     fd;
// 	short   events;
// 	short   revents;
// };

// std::vector< struct pollfd >	_fds;