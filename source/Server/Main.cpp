#include <Server.hpp>

#include "TestController.hpp"

int main(int argc, char *argv[]){
	Server* socket = new Server(3001);

	std::thread testController(TestControllerThread, socket);

	while (socket->IsOpen())
		socket->Update();

	testController.join();

	return 0;
}