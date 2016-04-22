#include <Server.hpp>

#include "ServerChat.hpp"
#include "ServerGame.hpp"

int main(int argc, char *argv[]){
	Server* socket = new Server(3001);

	std::thread chatView(ChatViewThread, socket);
	std::thread gameViewController(GameControllerThread, socket);

	while (socket->IsOpen())
		socket->Update();

	chatView.join();
	gameViewController.join();

	return 0;
}