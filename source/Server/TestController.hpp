#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>

void TestControllerThread(Socket* socket);

class TestController : ViewController{
public:
	TestController(Socket* socket);

	void OnData(const DataMap& data, int client) override;

	void OnMessage(std::string message, int client) override;

	void Update();
};