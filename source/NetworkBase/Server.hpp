#pragma once

#include "Socket.hpp"
#include <mutex>

class Server : public Socket{
protected:
	void _Recv() override;
	void _Send(ViewController* view, int exclude = -1, int exclusive = -1) override;
	void _Message(std::string key, std::string data, int exclude = -1, int exclusive = -1) override;

	unsigned int _maxClients = 2;
	std::unordered_map<unsigned int, TCPsocket> _clients;

	int _Register(TCPsocket socket);

	SDLNet_SocketSet _SocketSet();

public:
	Server(unsigned int port) : Socket(port){}

	virtual void Update();
};