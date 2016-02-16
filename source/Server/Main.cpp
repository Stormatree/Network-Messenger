#include "Server.hpp"

#include <iostream>

int main(int argc, char *argv[]){
	if (SDL_Init(0) == -1){
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		return 1;
	}

	Server server;

	server.open(1337, 4);

	getchar();
	
	server.close();
	
	SDL_Quit();

	return 0;
}