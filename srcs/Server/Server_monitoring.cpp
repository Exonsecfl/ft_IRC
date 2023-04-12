#include "Server.hpp"
#include <vector>
/*
bool Server::isClient(int fd) {

	std::vector< int >::iterator it;
	struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Obtenir les informations d'adresse et de port pour le socket client
    if (getpeername(fd, (struct sockaddr*)&client_addr, &client_addr_len) == -1) {
        std::cerr << "Erreur lors de l'appel de getpeername\n";
        return 1;
    }

    // Convertir l'adresse IP binaire en chaîne de caractères
    char client_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, INET_ADDRSTRLEN);

    // Afficher les informations d'adresse et de port pour le socket client
    std::cout << "Adresse IP du client : " << client_ip_str << "\n";
    std::cout << "Numéro de port du client : " << ntohs(client_addr.sin_port) << "\n";


	for (it = _client_fd.begin(); it != _client_fd.end(); it++)
		if ()

}
*/
void Server::monitoring( void )
{
	struct pollfd server = {_listener, POLLIN, 0};
	_fds[_nb_clients] = server;


	int	retpoll = -1;

	while (true) {

		//wait evenement
		retpoll = poll(_fds, _nb_clients + 1, TIMEOUT);
		if (retpoll == 0)
				throw std::runtime_error("[SERVER_MONITORING] - poll() timeout");
		else if (retpoll < 1)
				throw std::runtime_error("[SERVER_MONITORING] - ERROR poll()");

		for (int i = 0; i < MAX_CLIENTS + 1; i++)
			std::cout << "_fds fd: " << _fds[i].fd << " revents: " << _fds[i].revents << std::endl;

		//check a new connexion
		if (_fds[0].revents & POLLIN) {
		
			std::cout << "total client: " << _nb_clients << std::endl;
		if (_fds[0].revents & POLLIN)
		{
			std::cout << "je passe" << std::endl;
			if (_nb_clients == MAX_CLIENTS)
				break ;//manage max client

			socklen_t addrlen = sizeof(_addrclients[_nb_clients]);
			int client_fd = accept(_listener, &_addrclients[_nb_clients], &addrlen);
			if (client_fd == -1)
				throw std::runtime_error("[SERVER_MONITORING] - ERROR binding() failed");
			_fds[_nb_clients + 1].fd = client_fd;
			_fds[_nb_clients + 1].events = POLLIN;
			_nb_clients++;
			_client_fd.push_back(client_fd);
			continue ;
			if (!this->treatment_new(client_fd))
			{

				_fds[_nb_clients + 1].fd = client_fd;
				_fds[_nb_clients + 1].events = POLLIN;
				_nb_clients++;
				_client_fd.push_back(client_fd);
				std::cout  << "client cree " << _fd_nick_list[client_fd] << std::endl;
			}
			// memset(&_buffer,0,256);
			// recv(_fds[0].fd, _buffer, sizeof(_buffer), 0);
			std::cout << "buffer : " << _buffer << std::endl;
			continue;
		}

		std::cout << "buffer avant recv: " << _buffer << std::endl;

		//check event on fd_client if there are a data available
		for (int i = 1; i != _nb_clients + 1; i++) {

			if (_fds[i].revents == POLLIN) {
				std::cout << _fds[i].fd << std::endl;

			/****/
				memset(&_buffer,0,256);
				int res = recv(_fds[i].fd, _buffer, sizeof(_buffer), 0);
				std::cout << "res : " << res << std::endl;
				if (res < 0)
					throw std::runtime_error("[SERVER_MONITORING] - ERROR recv() failed");

				if (res == 0)
				{
					// supprimer le client et enlever la donner des 2 map
					std::cout << "deconnection du fd : " << _fds[i].fd << std::endl;
					close(_fds[i].fd);
					_fds[i].revents = -1;
					exit(0);

				}
				//std::cout << "res : " << res << std::endl;
				std::cout << _fds[i].fd << "[Client->Server]" << this->_buffer << std::endl;

				std::string command = this->parse(this->_buffer, _fds[i].fd);

				// temp value, to be replaced by a call to client
				// std::string nickname = "exo";

				if (command.find("CAP ",0) == 0)
				{
					this->Cmds_CAP(_fds[i].fd, _fd_nick_list[_fds[i].fd]);
				}

				if (command.find("PING", 0) == 0)
				{
					this->Cmds_ping(_fds[i].fd);
				}

				if (command.find("NICK", 0) == 0)
				{

				}

				if (command.find("QUIT", 0) == 0)
				{
					// deconnecter le client
					//continue;
					std::cout << "QUIT deconnection du fd : " << _fds[i].fd << std::endl;
					close(_fds[i].fd);
					_fds[i].revents = -1;
					exit(0);
				}

				if (command.find("squit", 0) == 0)
				{
					// deconnecter le client
					break;
				}
				std::cout << "------------------------------------- " <<  std::endl;
			}
			else if (_fds[i].revents == 17)
				exit(0);
		}

			/****/
	}

//	close(_listener);
//	close() //all fd_client
}
