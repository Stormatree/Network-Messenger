#pragma once

#include <SDL_net.h>
#include <iostream>
#include <vector>
#include <thread>
#include <unordered_map>

class Server{
	IPaddress _ip;
	unsigned int _port;
	TCPsocket _socket;
	
	std::unordered_map<unsigned int, TCPsocket> _clients;

	unsigned int _maxClients;

	std::string _password = "";

	std::thread _thread;

	bool _open = false;

	void _register(TCPsocket socket);

	void _send(unsigned int client, const std::string& data);
	void _recieve(unsigned int client, const std::string& data);

public:
	Server();
	~Server();

	bool open(unsigned int port, unsigned int maxClients = 8, std::string password = "");

	void close();

	friend void server_thread(Server* instance);
};

void server_thread(Server* instance);