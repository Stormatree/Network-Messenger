#pragma once

#include "Socket.hpp"

#include <thread>
#include <unordered_map>

class Server : public Socket{
	unsigned int _maxClients;
	std::unordered_map<unsigned int, TCPsocket> _clients;

	std::thread _thread;
	
	void _checkServer();
	void _checkClients();

	SDLNet_SocketSet _socketSet();

	int _register(TCPsocket socket);

	void _sendAll(const std::string& message);
	void _sendExcluding(unsigned int client, const std::string& message);

public:
	bool open(unsigned int port, unsigned int maxClients = 8);

	void close();

	friend void serverThread(Server* server);
};

void serverThread(Server* server);