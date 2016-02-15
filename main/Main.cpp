#include "Client.hpp"
#include "Server.hpp"

#include <string>
#include <iostream>

int main(int argc, char *argv[]){
	if (SDL_Init(0) == -1){
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		return 1;
	}

	Server server;

	server.open(1337, 4);

	getchar();


	//Client client;
	//
	//if (client.open("localhost", 1337)){
	//	while (1){
	//		std::string message;
	//
	//		std::cout << ">>> ";
	//		std::cin >> message;
	//
	//		if (message == "q")
	//			break;
	//
	//		client.send(message);
	//
	//		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	//	}
	//
	//	client.close();
	//}

	SDL_Quit();

	return 0;
}