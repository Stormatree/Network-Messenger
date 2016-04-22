#include "Socket.hpp"

#include "Encoder.hpp"

#include <iostream>

std::mutex Socket::_m;
std::condition_variable Socket::_cv;

std::atomic<bool> Socket::_recieved = false;
std::atomic<bool> Socket::_processing = false;

void Socket::_AddView(ViewController* view){
	if (_views.find(view->_key) == _views.end())
		_views[view->_key].insert(view);
}

const Socket::ViewSet& Socket::_GetViews(std::string key){
	auto view = _views.find(key);

	if (view != _views.end())
		return view->second;

	std::cout << "Socket.cpp, something went horribly wrong!\n";

	return ViewSet();
}

bool Socket::_ViewExists(std::string key){
	auto view = _views.find(key);

	if (view != _views.end())
		return true;

	return false;
}

void Socket::_Open(unsigned int port, std::string host){
	_Close();

	_port = port;
	_host = host;

	bool failed = false;

	if (_host == ""){
		if (SDLNet_ResolveHost(&_ip, nullptr, _port) == -1)
			failed = true;
	}
	else if (_host != ""){
		if (SDLNet_ResolveHost(&_ip, _host.c_str(), _port) == -1)
			failed = true;
	}

	if (failed){
		std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
		return;
	}

	while (_socket == nullptr){
		_socket = SDLNet_TCP_Open(&_ip);

		if (_socket == nullptr)
			std::cout << SDLNet_GetError() << "...\n";
	}

	_open = true;
}

void Socket::_Close(){
	if (_open){
		SDLNet_TCP_Close(_socket);
		_open = false;
	}
}

bool Socket::_SendRaw(TCPsocket socket, std::string message){
	if (message == "")
		return true;

	int size = message.length() * sizeof(char);

	int result = SDLNet_TCP_Send(socket, message.c_str(), size);

	if (result < size)
		return false;

	return true;
}

bool Socket::_RecvRaw(TCPsocket socket, std::string& message){
	char raw[MAX_MESSAGE];
	int maxSize = sizeof(raw);

	memset(&raw[0], 0, maxSize);

	int size = SDLNet_TCP_Recv(socket, raw, maxSize);

	if (size <= 0)
		return false;

	message = std::string(raw, size);

	return true;
}

void Socket::_ProcessData(std::string data, int client){
	std::string first = "";
	std::string second = "";

	if (Encoder::SplitSections(data, first, second)){
		_ProcessData(second);

		data = first;
	}

	std::string key;
	unsigned int type;

	Encoder::DecodeKeyType(data, key, type);

	if (type == Encoder::ERROR || !_ViewExists(key))
		return;

	ViewController::DataMap map;
	std::string message;

	if (type == Encoder::AMENDMENT)
		Encoder::DecodeAmendments(data, map);
	else if (type == Encoder::MESSAGE)
		Encoder::DecodeMessage(data, message);

	_recieved = true;
	_processing = false;

	_cv.notify_one();

	std::unique_lock<std::mutex> lk(_m);
	_cv.wait(lk, []{return _processing == true;});

	if (type == Encoder::AMENDMENT){
		const ViewSet viewSet = _GetViews(key);

		for (auto i : viewSet)
			i->OnData(map, client);
	}
	else if (type == Encoder::MESSAGE){
		const ViewSet viewSet = _GetViews(key);

		for (auto i : viewSet)
			i->OnMessage(message, client);
	}

	_recieved = false;
	_processing = false;

	lk.unlock();

	_cv.notify_one();
}

void Socket::_SendAllData(TCPsocket socket, int client){
	for (auto i : _views){
		size_t size = i.second.size();

		if (size > 0){
			_SendRaw(socket, Encoder::EncodeMessage((*i.second.begin())->Key(), std::to_string(client)));
			_SendRaw(socket, Encoder::EncodeAmendments((*i.second.begin()), true));
		}
	}
}

Socket::Socket(unsigned int port, std::string host){
	static bool sdl = false;

	if (!sdl){
		SDLNet_Init();
		sdl = true;
	}

	_Open(port, host);
}

Socket::~Socket(){
	_Close();

	SDLNet_Quit();
}

bool Socket::IsOpen(){
	return _open;
}

void Socket::PotentialWait(){
	if (_recieved){
		_processing = true;

		_cv.notify_one();

		std::unique_lock<std::mutex> lk(_m);
		_cv.wait(lk, []{return _processing == false;});
	}
}
