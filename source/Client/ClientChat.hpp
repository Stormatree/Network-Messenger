#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>
#include <CheckersDefines.hpp>

using namespace CheckersDefines;

void ChatViewThread(Socket* socket);

void ChatControllerThread(Socket* socket);

class ChatView : ViewController{
	int _client = -1;

public:
	ChatView(Socket* socket) : ViewController(Key::Chat, socket){}
	
	void OnMessage(std::string message, int client = -1) override;
};

class ChatController : ViewController{
public:
	ChatController(Socket* socket) : ViewController(Key::Chat, socket){}

	void Update();
};