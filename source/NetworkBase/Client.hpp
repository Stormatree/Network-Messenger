#pragma once

#pragma once

#include "Socket.hpp"

class Client : public Socket{
protected:
	void _Recv() override;
	void _Send(ViewController* view, int exclude = -1, int exclusive = -1) override;
	void _Message(std::string key, std::string data, int exclude = -1, int exclusive = -1) override;

public:
	Client(unsigned int port, std::string host) : Socket(port, host){}

	virtual void Update();
};