#include "TestController.hpp"

#include <iostream>

void TestControllerThread(Socket* socket){
	TestController* controller = new TestController(socket);

	while (socket->IsOpen()){
		Socket::PotentialWait();
		controller->Update();
	}
}

TestController::TestController(Socket* socket) : ViewController("test", socket){
	for (int i = 0; i < 8; i++)
		_SetData(i, "0");
}

void TestController::OnData(const DataMap& data, int client){
	if (client != 0)
		return;
	
	for (auto value : data)
		_SetData(value.first, value.second);
	
	_SendAmendments();
}

void TestController::OnMessage(std::string message, int client){
	_SendMessage(message, -1, client);
}

void TestController::Update(){
	system("cls");
	
	for (int i = 0; i < 8; i++)
		std::cout << _GetData(i) << "\n";
}
