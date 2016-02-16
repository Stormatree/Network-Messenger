#include "Client.hpp"
#include "Server.hpp"

#include <string>
#include <iostream>

//#define SERVER
#define CLIENT

int main(int argc, char *argv[]){
	if (SDL_Init(0) == -1){
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		return 1;
	}

#ifdef SERVER
	Server server;

	server.open(1337, 4);

	getchar();
#endif

#ifdef CLIENT
	Client client;
	
	if (client.open("localhost", 1337)){
		while (1){
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			std::string message;

			std::getline(std::cin, message);
	
			if (message == "q")
				break;
	
			client.send(message);
		}
	
		client.close();
	}
#endif

	SDL_Quit();

	return 0;
}