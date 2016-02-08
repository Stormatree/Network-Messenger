#pragma once

#include <SDL_net.h>
#include <string>

class Client{
	IPaddress _ip;
	unsigned int _port;
	TCPsocket _socket;

	bool _open = false;

public:
	Client();
	~Client();

	bool open(const std::string& host, int port);

	void send(const std::string& data);

	void close();
};