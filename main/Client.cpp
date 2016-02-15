#include "Client.hpp"

#include <iostream>

Client::Client(){
	if (SDLNet_Init() == -1){
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << "\n";
		return;
	}
}

Client::~Client(){
	close();
	SDLNet_Quit();
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

	return true;
}

void Client::send(const std::string& data){
	if (!_open){
		std::cout << "Client not open!\n";
		return;
	}

	int length = data.length() * sizeof(char);

	int result = SDLNet_TCP_Send(_socket, data.c_str(), length);

	if (result < length)
		std::cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << "\n";
}

void Client::close(){
	if (_open){
		SDLNet_TCP_Close(_socket);
		_open = false;
	}
}