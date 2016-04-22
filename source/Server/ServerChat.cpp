#include "ServerChat.hpp"

void ChatViewThread(Socket* socket){
	ChatView* chatView = new ChatView(socket);

	while (socket->IsOpen())
		Socket::PotentialWait();
}

void ChatView::OnMessage(std::string message, int client){
	std::cout << client << " - " << message << "\n";

	_SendMessage(message, client);
}