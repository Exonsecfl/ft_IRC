#include "Server.hpp"


void Server::Cmds_notice(int const fd_client, std::string const command)
{
	std::string hostname = this->_hostname;
	std::string args = find_cmd_arg(command, "NOTICE");
	size_t separation = args.find(":");
	std::string nick = this->_fd_nick_list[fd_client];
	if (separation == std::string::npos) // if not found - npos represents an invalid or not-found position
	{
		std::string cap_response = "the correct way to write is: PRIVMSG destination_nick -or- #destination_channel :message\r\n";
		std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
		send(fd_client, cap_response.c_str(), cap_response.length(), 0);
	}
	else
	{
		std::string dest = args.substr(0, separation - 1); // ATTENTION DEPEND DE L'HOTE DU SERVEUR
		std::string msg = args.substr(separation + 1);

		std::cout << "destination : " << dest << " msg : " << msg <<std::endl;

		if ( dest[0] == '#')
		{
			dest = dest.substr(1);

			if (this->_channels.count(dest) == 0)
			{
				// std::string cap_response = "403 the channel doesn't exist\r\n";
				std::string cap_response = "403\r\n";
				std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
				send(fd_client, cap_response.c_str(), cap_response.length(), 0);
				return ;
			}
			if (this->_channels[dest]->getChannelConnectedFDMode(fd_client) == "b" || this->_channels[dest]->getChannelConnectedFDMode(fd_client) == "")
			{
				// std::string cap_response = :kinetic.oftc.net 404 vanilou #loutre :Cannot send to channel
				std::string cap_response =  ":" + hostname + " 404 " + this->_clientList[_fd_nick_list[fd_client]]->getNickname() + " #" + dest;
				cap_response += " :Cannot send to channel\r\n";
				std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
				send(fd_client, cap_response.c_str(), cap_response.length(), 0);
				return ;
			}
			//std::map <std::string, std::string>    channelClients = this->_channels[dest]->getMapUsers();
			std::map <int, std::string>    channelClients = this->_channels[dest]->getChannelFDsModeMap();
			for(std::map <int, std::string>::iterator it = channelClients.begin() ;it != channelClients.end(); ++it)
			{
				int fd_dest = it->first;
				if (fd_dest != fd_client)
				{
					std::string cap_response = ":" + nick + " NOTICE #" + dest + " " + msg + "\r\n";
					std::cout << fd_dest << " [Server->Client]" << cap_response << std::endl;
					send(fd_dest, cap_response.c_str(), cap_response.length(), 0);
				}
			}
		}
		else
		{
			if (this->_clientList.count(dest) == 0)
			{
				std::string cap_response = "401\r\n";
				std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
				send(fd_client, cap_response.c_str(), cap_response.length(), 0);
				return ;
			}
			int fd_dest = this->_clientList[dest]->getClientFd();
			std::string cap_response = ":" + nick + " NOTICE " + dest + " " + msg + "\r\n";
			std::cout << fd_dest<< " [Server->Client]" << cap_response << std::endl;
			send(fd_dest, cap_response.c_str(), cap_response.length(), 0);
		}
	}
}
