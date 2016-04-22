#include "Server.hpp"

#include "Encoder.hpp"

#include <iostream>

void Server::_Recv(){
	// Check all existing clients
	std::list<unsigned int> dropped;

	for (auto client : _clients){
		if (SDLNet_SocketReady(client.second)){
			std::string data;

			if (_RecvRaw(client.second, data) == false){
				dropped.push_back(client.first);
				continue;
			}
			
			_ProcessData(data, client.first);
		}
	}

	// Erase dropped connections
	for (unsigned int i : dropped)
		_clients.erase(i);
}

void Server::_Send(ViewController* view, int exclude, int exclusive){
	for (auto i : _clients)
		_SendRaw(i.second, Encoder::EncodeAmendments(view, true));
}

void Server::_Message(std::string key, std::string data, int exclude, int exclusive){
	if (key == "" || data == "")
		return;

	if (exclusive != -1){
		_SendRaw(_clients[exclusive], Encoder::EncodeMessage(key, data));
		return;
	}

	for (auto i : _clients){
		if (i.first != exclude)
			_SendRaw(i.second, Encoder::EncodeMessage(key, data));
	}
}

int Server::_Register(TCPsocket socket){
	// Add incoming socket to _clients
	if (_clients.size() >= _maxClients)
		return -1;

	if (_clients.empty()){
		_clients[0] = socket;
		_SendAllData(socket, 0);

		return 0;
	}

	unsigned int i = 0;

	for (auto client : _clients){
		if (client.first != i){
			_clients[i] = socket;
			_SendAllData(socket, i);

			return i;
		}

		i++;
	}

	_clients[i] = socket;
	_SendAllData(socket, i);

	return i;
}

SDLNet_SocketSet Server::_SocketSet(){
	SDLNet_SocketSet set = SDLNet_AllocSocketSet(_clients.size() + 1);

	SDLNet_TCP_AddSocket(set, _socket);

	for (auto client : _clients)
		SDLNet_TCP_AddSocket(set, client.second);

	return set;
}

void Server::Update(){
	if (IsOpen()){
		SDLNet_SocketSet set = _SocketSet();

		int active = SDLNet_CheckSockets(set, 1000);

		SDLNet_FreeSocketSet(set);

		// Check for new clients
		if (_clients.size() < _maxClients && SDLNet_SocketReady(_socket)){
			TCPsocket incoming = SDLNet_TCP_Accept(_socket);

			if (incoming)
				_Register(incoming);
		}

		_Recv();
		// Check data from all clients
	}
}