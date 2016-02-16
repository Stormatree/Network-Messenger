#pragma once

#include <SDL_net.h>
#include <string>

#define MAX_MESSAGE 1024

class Socket{
protected:
	IPaddress _ip;
	unsigned int _port;
	TCPsocket _socket;

	bool _open = false;

	bool _sendMessage(TCPsocket, const std::string& message);

	bool _getMessage(TCPsocket socket, std::string& message);

public:
	Socket();
	virtual ~Socket();

	virtual void close(){}
};