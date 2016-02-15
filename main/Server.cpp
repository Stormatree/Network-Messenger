#include "Server.hpp"

#include <chrono>

void server_thread(Server* instance){
	while (instance->_open){
		if (instance->_clients.size() < instance->_maxClients){
			TCPsocket incoming = SDLNet_TCP_Accept(instance->_socket);

			if (incoming)
				instance->_register(incoming);
		}

		for (auto client : instance->_clients){
			char message[1024];

			int length = SDLNet_TCP_Recv(client.second, message, 1024);
			
			printf("Received: %.*s\n", length, message);
		}
	}

	// Close client connections
}

void Server::_register(TCPsocket socket){
	if (_clients.size() >= _maxClients)
		return;

	if (_clients.empty()){
		_clients[0] = socket;
		return;
	}

	unsigned int i = 0;

	for (auto client : _clients){
		if (client.first != i){
			_clients[i] = socket;
			return;
		}
		
		i++;
	}

	_clients[i] = socket;
}

void Server::_send(unsigned int client, const std::string& data){
	// Called when server sends data to client
}

void Server::_recieve(unsigned int client, const std::string& data){
	// Called when client sends data to server
}

Server::Server(){
	if (SDLNet_Init() == -1){
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << "\n";
		return;
	}
}

Server::~Server(){
	close();
	SDLNet_Quit();
}

bool Server::open(unsigned int port, unsigned int maxClients, std::string password){
	close();

	_port = port;
	_maxClients = maxClients;
	_password = password;

	if (SDLNet_ResolveHost(&_ip, nullptr, _port) == -1){
		std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
		return false;
	}

	_socket = SDLNet_TCP_Open(&_ip);

	if (!_socket){
		std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
		return false;
	}
	
	_open = true;

	_thread = std::thread(server_thread, this);
	_thread.detach();

	return true;
}

// After the detach (above), and before the join (below), this instance is only edited by the thread

void Server::close(){
	if (_open){
		_open = false;

		_thread.join();

		// Shut down server

		_clients.clear();
	}
}