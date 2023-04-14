#include "Server.hpp"

void Server::Cmds_join(int const fd_client, std::string const command, std::string const nickname)
{
	std::string pchannel = command.substr(5);

    if ("DEBUG" ==_IRCconfig->getConfigValue("DEBUG")) // -------------------------------------
	{
		// retrieve error code of getaddrinfo command 
		std::cout << BLU;
		std::cout << "[ Server::Cmds_join ]" << fd_client << std::endl;
        std::cout << "fd_client :" << fd_client << std::endl;
        std::cout << "channel :" << pchannel << std::endl;
    	std::cout << "nickname :" << nickname << std::endl; // WARNING missing info
		std::cout << NOC;
	} // --------------------------------------------------------------------------------------

	std::string nick = "vroch"; // to be deleted once the problem with nickname solved.
	// replace in the hereafter code nick by nickname

	

	std::string segment[10];
	std::string typeC[10];

	// initialise
	for (int i = 0 ; i < 10 ; i++)
	{
		segment[i] = "";
		typeC[i] = "";
	}

	// identify if manny chanels are transfered in one JOIN and sepparated by a comma
	if (pchannel.find(",")==0)
	{
		// extract all channels - max 10;
		
	}
	else
	{
		typeC[0] = pchannel.substr(0, 1);
		segment[0] = pchannel.substr(1);
	}
	
	// management fo the channels objects (one object per channel)


	
	// find if the channel is already defined
	std::map<std::string, Channel>::iterator it = this->_channels.find(segment[0]);
	bool channelAlreadyDefined = false;
	if (it != this->_channels.end())
	{
		// modifiy the existing object
		this->_channels.find(segment[0])->second.getNbConnection();
		channelAlreadyDefined = true;

	}
	else
	{
		// create a new map record and by consequence a new channel object
		this->_channels[segment[0]]=(Channel(segment[0]));
		// add the user to the list of connected users to this channel
		this->_channels.find(segment[0])->second.setConnectedUser(nick);

		

	}


	std::cout << "Object :" << this->_channels.find(segment[0])->second.getChannelName() << std::endl;
	std::cout << "users :" << this->_channels.find(segment[0])->second.getConnectedUsers() << std::endl;


	std::string hostname = this->_hostname;

    //std::string cap_response = "JOIN " + segment[0] + "\r\n";
    //std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;

    //send(fd_client, cap_response.c_str(), cap_response.length(), 0);

	// for the channel creator only
	if (channelAlreadyDefined == false)
	{
	//12:28 -!- vroch [~vroch@freenode-o6d.g28.dc9e5h.IP] has joined #blabla
	//12:28 [Users #blabla]
	//12:28 [@vroch]
	//12:28 -!- Irssi: #blabla: Total of 1 nicks [1 ops, 0 halfops, 0 voices, 0         normal]
	//12:28 -!- Channel #blabla created Fri Apr 14 12:28:33 2023
	//12:28 -!- Irssi: Join to #blabla was synced in 0 secs

	std::string cap_response;
	
	//cap_response = "JOIN " + typeC[0] + segment[0] + "\r\n";
    //std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
	
	//send(fd_client, cap_response.c_str(), cap_response.length(), 0);

	cap_response = nickname + "!" + nickname + '@' + hostname + " JOIN " + typeC[0] + segment[0] + "\r\n";
    std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
	
	send(fd_client, cap_response.c_str(), cap_response.length(), 0);

	

	cap_response = "332 " + typeC[0] + segment[0] + " :this is my channel \r\n";
    std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
	
	send(fd_client, cap_response.c_str(), cap_response.length(), 0);


	//353     RPL_NAMREPLY     "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	cap_response = "353 " + typeC[0] + segment[0] + ": [@" + nick + "]\r\n";
    std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
	
	send(fd_client, cap_response.c_str(), cap_response.length(), 0);


	
	//cap_response = ":c1r6s4.42lausanne.42 366 vroch #blabla :End of NAMES list";
    //std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
	
	//send(fd_client, cap_response.c_str(), cap_response.length(), 0);
	}


	//send(fd_client, cap_response.c_str(), cap_response.length(), 0);
	

	}