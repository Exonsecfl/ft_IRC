#include "Server.hpp"

void Server::Cmds_part(int const fd_client, std::string const command, std::string const nickname)
{
	std::string pchannel = "";
    // temporary code
	if (command.find("PART") == 0)
		pchannel = command.substr(5);
	else
		pchannel = command;
	//end of temporary code

	std::string hostname = this->_hostname;

	int max_segment = MAX_JOINS_PER_LINE;
	std::string segment[max_segment];
	std::string typeC[max_segment];
	

	// initialise the MAX_JOINS_PER_LINE potentitial new join
	for (int i = 0 ; i < max_segment ; i++)
	{
		segment[i] = "";
		typeC[i] = "";
	}

	// identify if manny chanels are transfered in one JOIN and separated by a comma, no spaces allowed
	if (pchannel.find(",") < pchannel.size())
	{
		// read each iteration of channels in one command
		std::cout << YEL << " find " << NOC << std::endl;
		for (int i = 0 ; i < max_segment ; i++)
		{
			std::cout << YEL << i << " find p =" << pchannel.find(",", 0) << NOC << std::endl;
			if (pchannel.find(",", 0) < pchannel.size())
			{
				typeC[i] = pchannel.substr(0, 1);
				segment[i] = pchannel.substr(1, pchannel.find(",")-1);
				// reduce the size of the pchannel for the next cycle
				pchannel = pchannel.substr(pchannel.find(",")+1);
			}
			else
			{
				typeC[i] = pchannel.substr(0, 1);
				segment[i] = pchannel.substr(1, pchannel.find("\r")-1);
				pchannel = "";
				break;
			}
		}
	}
		
	for (int i = 0 ; i < max_segment ; i++ )
	{
		if (segment[i] == "")
			break;

		// Find IP address
		std::string ip_client = this->_clientList[nickname]->get_ip();

		// send first message informing about quit user e.g. :
		// :exo_b!exo_b@127.0.0.1 PART #blabla
		std::string cap_response = ":" + nickname + "!" + nickname + '@' + ip_client + " PART " + typeC[i] + segment[i] + "\r\n";

		send(fd_client, cap_response.c_str(), cap_response.length(), 0);


		// **** delete channel's user and/or channel itself

		// retieve the user Mode to ensure he's not banned
		std::map<std::string, Channel*>::iterator it = _channels.find(segment[i]);
		if (it != _channels.end() && it->second->getConnectedUsersMode(nickname) != "b")		{
			// delete the user
			it->second->resetConnectedUser(nickname);
		}
		// check if the user deleted whas the last one (exclude banned users)
		if (it->second->getNbUsers() == 0)
		{
		// delete the channel
		// ?? delete (it)


		}
	}

}