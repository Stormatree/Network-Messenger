#include "ClientChat.hpp"
#include "ClientGame.hpp"

#include <Client.hpp>

int main(int argc, char *argv[]){
	Client* socket = new Client(3001, "localhost");

	std::thread chatView(ChatViewThread, socket);
	std::thread chatController(ChatControllerThread, socket);
	std::thread gameViewController(GameViewControllerThread, socket);

	while (socket->IsOpen())
		socket->Update();

	chatView.join();
	chatController.join();
	gameViewController.join();

	return 0;
}