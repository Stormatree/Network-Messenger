#include "TestView.hpp"

#include <iostream>
#include <random>
#include <chrono>
#include <thread>

const std::string chars[2] = {
	"AEIO  eiouA  Oua",
	"b  FhJKLmnP StVY"
};

std::random_device rd;

std::mt19937 gen(rd());

std::uniform_int_distribution<> rnge(0, 15);

std::string Jumbler(unsigned int length){
	std::string jumbled = "";

	unsigned int flip = 0;

	for (unsigned int i = 0; i < length; i++){
		jumbled += chars[flip].at(rnge(gen));

		if (flip == 0)
			flip = 1;
		else
			flip = 0;
	}

	return jumbled;
}

unsigned long Now(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void TestViewThread(Socket* socket){
	TestView* controller = new TestView(socket);

	while (socket->IsOpen()){
		Socket::PotentialWait();
		controller->Update();
	}
}

TestView::TestView(Socket * socket) : ViewController("test", socket){
	for (int i = 0; i < 8; i++)
		_SetData(i, "0");
}

void TestView::OnData(const DataMap & data, int client){
	unsigned long now = Now();

	_dTime = now - _dSent;

	ViewController::OnData(data, client);
}

void TestView::OnMessage(std::string message, int client){
	if (_id == -1){
		_id = std::stoi(message);
		return;
	}

	unsigned long now = Now();

	_mTime = now - _mSent;
}

void TestView::Update(){
	system("cls");

	for (int i = 0; i < 8; i++)
		std::cout << _GetData(i) << "\n";

	std::cout << "\nData: " << _dTime << " ms\nMessage: " << _mTime << " ms\n";

	unsigned long now = Now();
	
	if (now % 50 == 0){
		_mSent = now;
		_dSent = now;

		// sending messages
		
		_SendMessage(std::to_string(now));

		// sending data 

		for (int i = 0; i < 8; i++)
			_SetAmendment(i, Jumbler(16));

		_SendAmendments();
	}
}
