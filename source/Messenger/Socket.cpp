#include "Socket.hpp"

#include <iostream>

bool Socket::_sendMessage(TCPsocket socket, const std::string& message){
	int size = message.length() * sizeof(char);

	int result = SDLNet_TCP_Send(socket, message.c_str(), size);

	if (result < size)
		return false;

	return true;
}

bool Socket::_getMessage(TCPsocket socket, std::string& message){
	char raw[MAX_MESSAGE];

	int size = SDLNet_TCP_Recv(socket, raw, MAX_MESSAGE * sizeof(char));

	if (size <= 0){
		return false;
	}

	message = std::string(raw, size / sizeof(char));

	return true;
}

Socket::Socket(){
	if (SDLNet_Init() == -1){
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << "\n";
		return;
	}
}

Socket::~Socket(){
	close();

	SDLNet_Quit();
}