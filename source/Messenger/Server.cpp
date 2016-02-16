#include "Server.hpp"

#include <iostream>
#include <string>

void serverThread(Server* server){
	while (server->_open){

		SDLNet_SocketSet set = server->_socketSet();

		int active = SDLNet_CheckSockets(set, 1000);

		SDLNet_FreeSocketSet(set);

		if (active < 0)
			break;

		if (active == 0)
			continue;

		server->_checkServer();

		server->_checkClients();
	}
}

SDLNet_SocketSet Server::_socketSet(){
	SDLNet_SocketSet set = SDLNet_AllocSocketSet(_clients.size() + 1);

	SDLNet_TCP_AddSocket(set, _socket);

	for (auto client : _clients){
		SDLNet_TCP_AddSocket(set, client.second);
	}

	return set;
}

void Server::_checkServer(){
	// If new client incoming
	if (SDLNet_SocketReady(_socket) && _clients.size() < _maxClients){
		TCPsocket incoming = SDLNet_TCP_Accept(_socket);

		if (incoming){
			unsigned int i = _register(incoming);

			std::string message = "Client " + std::to_string(i) + " joined!";

			std::cout << message << "\n";

			_sendExcluding(i, message);
		}
	}
}

void Server::_checkClients(){
	// Check all existing clients
	std::list<unsigned int> dropped;

	for (auto client : _clients){
		if (SDLNet_SocketReady(client.second)){
			std::string message;

			if (_getMessage(client.second, message) == false){
				dropped.push_back(client.first);
				continue;
			}

			message = "Client " + std::to_string(client.first) + " says: " + message;

			std::cout << message << "\n";

			_sendExcluding(client.first, message);
		}
	}

	// Erase dropped connections
	for (unsigned int i : dropped){
		_clients.erase(i);

		std::string message = "Client " + std::to_string(i) + " left...";

		std::cout << message << "\n";

		_sendAll(message);
	}
}

void Server::_sendAll(const std::string& message){
	for (auto i : _clients){
		_sendMessage(i.second, message);
	}
}

void Server::_sendExcluding(unsigned int client, const std::string& message){
	for (auto i : _clients){
		if (i.first == client)
			continue;

		_sendMessage(i.second, message);
	}
}

int Server::_register(TCPsocket socket){
	if (_clients.size() >= _maxClients)
		return -1;

	if (_clients.empty()){
		_clients[0] = socket;
		return 0;
	}

	unsigned int i = 0;

	for (auto client : _clients){
		if (client.first != i){
			_clients[i] = socket;
			return i;
		}
		
		i++;
	}

	_clients[i] = socket;
	return i;
}

bool Server::open(unsigned int port, unsigned int maxClients){
	close();

	_port = port;
	_maxClients = maxClients;

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

	_clients.reserve(_maxClients);

	_thread = std::thread(serverThread, this);
	
	return true;
}

void Server::close(){
	if (_open){
		_open = false;

		_thread.join();

		_clients.clear();
	}
}