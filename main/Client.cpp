#include "Client.hpp"

#include <iostream>

void clientThread(Client* client){
	while (client->_open){

		std::string message;

		if (client->_getMessage(client->_socket, message) == false)
			break;

		std::cout << message << "\n";
	}
}

bool Client::open(const std::string& host, unsigned int port){
	close();

	_port = port;

	if (SDLNet_ResolveHost(&_ip, host.c_str(), _port) == -1){
		std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
		return false;
	}

	_socket = SDLNet_TCP_Open(&_ip);

	if (!_socket){
		std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
		return false;
	}

	_open = true;

	_thread = std::thread(clientThread, this);

	return true;
}

void Client::send(const std::string& data){
	_sendMessage(_socket, data);

	// Should probably make this push to some sort of stack buffer, for later use in thread
}

void Client::close(){
	if (_open){
		_open = false;

		_thread.join();

		SDLNet_TCP_Close(_socket);
	}
}