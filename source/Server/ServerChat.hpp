#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>
#include <CheckersDefines.hpp>

#include <iostream>

using namespace CheckersDefines;

void ChatViewThread(Socket* socket);

class ChatView : ViewController{
public:
	ChatView(Socket* socket) : ViewController(Key::Chat, socket){}

	void OnMessage(std::string message, int client = -1) override;
};