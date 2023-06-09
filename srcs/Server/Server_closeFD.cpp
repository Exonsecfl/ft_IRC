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
		std::stringstream ss;
		ss << "[SERVER_CLOSEFD] - Succesfully closed fd nb[" << fd << "]";
		fd = -1;
	}
	else
		std::cout << ("[SERVER_CLOSEFD] - WARN - try to close fd -1");
}