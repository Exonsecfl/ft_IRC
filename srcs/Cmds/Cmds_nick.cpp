#include "Server.hpp"

void Server::Cmds_nick(int const fd_client, std::string const command)
{
	std::string newNick = command;
	// std::vector<std::string>	channels = this->_clientList[_fd_nick_list[fd_client]]->getChannel();
	std::string	oldNickname;
	oldNickname = this->_clientList[_fd_nick_list[fd_client]]->getNickname();

    if ("DEBUG" ==_IRCconfig->getConfigValue("DEBUG")) // -------------------------------------
	{
		// retrieve error code of getaddrinfo command
		std::cout << BLU;
		std::cout << "[Server::Cmds_nick]" << std::endl;
        std::cout << " fd_client :" << fd_client << std::endl;
        std::cout << " old nick :" << oldNickname << std::endl;
		std::cout << " new nick :" << newNick  << std::endl;
		std::cout << NOC << std::endl;;
	} // --------------------------------------------------------------------------------------

	std::map<std::string, Client>::iterator it;
	if (_clientList.count(newNick))
	{
		std::string cap_response = "433 Nickname already exist, please choose another\r\n";
		std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
		send(fd_client, cap_response.c_str(), cap_response.length(), 0);
		std::cout << "Nickname already used" << std::endl;
	}
	else if (newNick.length() < 1)
	{
		std::string cap_response = "431 No new nickname given\r\n";
		std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
		send(fd_client, cap_response.c_str(), cap_response.length(), 0);
		std::cout << "No new nickname given" << std::endl;
	}
	else if (newNick.length() > 20 || newNick.length() < 3)
	{
		std::string cap_response = "432 " + newNick + "Nickname " + newNick + " does not respond to standard \r\n";
		std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;
		send(fd_client, cap_response.c_str(), cap_response.length(), 0);
		std::cout << "*" << newNick << "*" << "Nickname does not respond to standard length: " << + newNick.length() << std::endl;
	}
	else
	{
		// update of client and maps _clientList, _fd_nick_list
		this->_clientList[oldNickname]->setNickname(newNick);
		Client	*tempClient = _clientList[oldNickname];
		_clientList.erase(oldNickname); //delete the actualclient
		_clientList.insert(std::pair<std::string, Client *>(newNick, tempClient));
		_fd_nick_list[fd_client] = newNick;
		std::string cap_response;
		if (oldNickname == "#")
			cap_response = "001 " + newNick + " Your nickname is: " + newNick + "\r\n";
		else
		{
			cap_response = ":" + oldNickname + "!~" + this->_clientList[_fd_nick_list[fd_client]]->get_user() + "@";
			cap_response += this->_clientList[_fd_nick_list[fd_client]]->get_ip() + " NICK :" + newNick + "\r\n";
		}
		std::cout << fd_client << " [Server->Client]own" << cap_response << std::endl;
		send(fd_client, cap_response.c_str(), cap_response.length(), 0);
	// inform the other users in contact:

		std::vector<int> contacts = _clientList[_fd_nick_list[fd_client]]->getContactsFd();
		for (std::vector<int>::iterator it_fd = contacts.begin(); it_fd != contacts.end() ; ++it_fd)
		{
				std::cout << *it_fd << " [Server->Client]" << cap_response << std::endl;
				send(*it_fd, cap_response.c_str(), cap_response.length(), 0);
		}

		std::map<std::string, Channel*>::iterator it = this->_channels.begin();

		if (it == this->_channels.end())
			return;

		for ( ; it != this->_channels.end() ; it++)
		{
			if (it->second->getChannelConnectedFD(fd_client) == fd_client) //client connected to this channel
			{
				std::map <int, std::string>    channelClients = it->second->getChannelFDsModeMap();
				for(std::map <int, std::string>::iterator it = channelClients.begin() ;it != channelClients.end(); ++it)
				{
					int fd_dest = it->first;
					if (fd_dest != fd_client)
					std::cout << "fd_dest " << fd_dest << std::endl;
					if (fd_dest != fd_client && !find_in_vector<int>(contacts, fd_dest))
					{
						contacts.push_back(fd_dest);
						std::cout << fd_dest << " [Server->Client]" << cap_response << std::endl;
						send(fd_dest, cap_response.c_str(), cap_response.length(), 0);
					}
				}
			}
		}

		std::cout << " change of Nick fd: " << fd_client << "new nick : " << _fd_nick_list[fd_client] << std::endl;
	}
}

/*

Code de réponse 001 (RPL_WELCOME) : Il s'agit d'une réponse positive indiquant que le serveur a accepté le changement de pseudonyme et que l'utilisateur est maintenant connecté sous le nouveau pseudonyme. Le message peut ressembler à ceci : "001 : Bienvenue sur le serveur IRC. Votre pseudo est maintenant [nouveau_pseudonyme]".
Code de réponse 431 (ERR_NONICKNAMEGIVEN) : Il s'agit d'une réponse d'erreur indiquant que le client n'a pas fourni de pseudonyme dans la commande "NICK". Le message peut ressembler à ceci : "431 : Erreur - aucun pseudonyme fourni. Veuillez spécifier un pseudonyme valide".
Code de réponse 432 (ERR_ERRONEUSNICKNAME) : Il s'agit d'une réponse d'erreur indiquant que le pseudonyme fourni par le client dans la commande "NICK" est invalide selon les règles de syntaxe du protocole IRC. Le message peut ressembler à ceci : "432 : Erreur - pseudonyme invalide. Veuillez spécifier un pseudonyme valide".
Code de réponse 433 (ERR_NICKNAMEINUSE) : Il s'agit d'une réponse d'erreur indiquant que le pseudonyme fourni par le client dans la commande "NICK" est déjà utilisé par un autre utilisateur sur le serveur IRC. Le message peut ressembler à ceci : "433 : Erreur - pseudonyme déjà utilisé. Veuillez choisir un autre pseudonyme".

*/
