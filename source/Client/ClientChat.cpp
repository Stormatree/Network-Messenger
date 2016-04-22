#include "ClientChat.hpp"

#include <iostream>

void ChatViewThread(Socket* socket){
	ChatView* chatView = new ChatView(socket);

	while (socket->IsOpen())
		Socket::PotentialWait();
}

void ChatControllerThread(Socket* socket){
	ChatController* chatView = new ChatController(socket);

	while (socket->IsOpen()){
		Socket::PotentialWait();
		chatView->Update();
	}
}

void ChatView::OnMessage(std::string message, int client){
	if (_client == -1){
		_client = std::stoi(message);

		std::cout << "Weclome player " << _client << ".\nUse this command line to chat to your opponent.\n\n";
		return;
	}

	std::cout << message << "\n";
}

void ChatController::Update(){
	std::string message;

	std::getline(std::cin, message);

	_SendMessage(message);
}