#pragma once

#include "Socket.hpp"

#include <string>
#include <thread>

class Client : public Socket{
	std::thread _thread;

public:
	bool open(const std::string& host, unsigned int port);

	void send(const std::string& data);

	void close();

	friend void clientThread(Client* server);
};

void clientThread(Client* client);