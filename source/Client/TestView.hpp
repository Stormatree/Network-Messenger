#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>

void TestViewThread(Socket* socket);

class TestView : ViewController{
	int _id = -1;

	std::string _message = "";

	unsigned long _mSent = 0;
	unsigned long _dSent = 0;

	unsigned long _mTime = 0;
	unsigned long _dTime = 0;

public:
	TestView(Socket* socket);

	void OnData(const DataMap& data, int client) override;

	void OnMessage(std::string message, int client) override;

	void Update();
};