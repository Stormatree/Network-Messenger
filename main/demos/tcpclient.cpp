#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>
#include <string>
#include <iostream>

// Edited demo from original version included in official docs

int main(int argc, char *argv[]){
	std::string hostStr = "";
	std::string portStr = "";

	std::cout << "Host: ";
	std::cin >> hostStr;

	std::cout << "Port: ";
	std::cin >> portStr;

	IPaddress ip;
	Uint16 port = (Uint16)std::stoi(portStr);

	TCPsocket sock;

	char message[1024];
	int len;	

	/* initialize SDL */
	if(SDL_Init(0) == -1){
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		return 1;
	}

	/* initialize SDL_net */
	if(SDLNet_Init() == -1){
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << "\n";
		return 1;
	}

	if (SDLNet_ResolveHost(&ip, hostStr.c_str(), port) == -1){
		std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
		return 1;
	}
	
	/* open the server socket */
	sock = SDLNet_TCP_Open(&ip);

	if(!sock){
		std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << "\n";
		return 1;
	}

	/* read the buffer from stdin */
	std::cout << "Enter Message, or Q to make the server quit:\n";

	fgets(message, 1024, stdin);
	len = strlen(message);

	/* strip the newline */
	message[len-1] = '\0';
	
	if(len){
		int result;
		
		/* print out the message */
		printf("Sending: %.*s\n", len, message);

		result = SDLNet_TCP_Send(sock, message, len); /* add 1 for the NULL */

		if(result<len)
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
	}

	SDLNet_TCP_Close(sock);
	
	/* shutdown SDL_net */
	SDLNet_Quit();

	/* shutdown SDL */
	SDL_Quit();

	return 0;
}