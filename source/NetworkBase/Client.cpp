#include "Client.hpp"

#include "Encoder.hpp"

#include <iostream>

void Client::_Recv(){
	std::string data;

	if (_RecvRaw(_socket, data) == false)
		_Close();

	_ProcessData(data);
}

void Client::_Send(ViewController* view, int exclude, int exclusive){
	_SendRaw(_socket, Encoder::EncodeAmendments(view));
}

void Client::_Message(std::string key, std::string data, int exclude, int exclusive){
	_SendRaw(_socket, Encoder::EncodeMessage(key, data));
}

void Client::Update(){
	if (IsOpen())
		_Recv();
}