#include "Client.hpp"

#include <string>
#include <iostream>

int main(int argc, char *argv[]){
	if (SDL_Init(0) == -1){
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		return 1;
	}

	Client client;
	
	if (client.open("localhost", 1337)){
		while (1){
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			std::string message;

			std::getline(std::cin, message);
	
			if (message == "q")
				break;
	
			client.send(message);
		}
	
		client.close();
	}
	
	SDL_Quit();

	return 0;
}