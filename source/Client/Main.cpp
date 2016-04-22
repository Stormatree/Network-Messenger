#include <Client.hpp>

#include "TestView.hpp"

#include <iostream>

int main(int argc, char *argv[]){
	std::string host;

	std::cout << "Entet Host: ";

	std::cin >> host;

	Client* socket = new Client(3001, host);

	std::thread testView(TestViewThread, socket);

	while (socket->IsOpen())
		socket->Update();

	testView.join();

	return 0;
}