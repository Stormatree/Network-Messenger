#pragma once

#include "ViewController.hpp"

#include <SDL_net.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <set>

#define MAX_MESSAGE 1024

class Socket{
	using ViewSet = std::set<ViewController*>;
	using ViewSetMap = std::unordered_map<std::string, ViewSet>;

	std::unordered_map<std::string, std::set<ViewController*>> _views;
	std::atomic<bool> _open = false;

	void _AddView(ViewController* view);
	bool _ViewExists(std::string key);

	unsigned int _port;
	std::string _host;

protected:
	IPaddress _ip;
	TCPsocket _socket = nullptr;

	static std::mutex _m;
	static std::condition_variable _cv;

	static std::atomic<bool> _recieved;
	static std::atomic<bool> _processing;

	const ViewSet& _GetViews(std::string key);

	void _Open(unsigned int port, std::string host = "");
	void _Close();

	bool _SendRaw(TCPsocket, std::string message);
	bool _RecvRaw(TCPsocket socket, std::string& message);

	void _ProcessData(std::string data, int client = -1);

	void _SendAllData(TCPsocket socket, int client = -1);

	virtual void _Recv() = 0;
	virtual void _Send(ViewController* view, int exclude = -1, int exclusive = -1) = 0;
	virtual void _Message(std::string key, std::string data, int exclude = -1, int exclusive = -1) = 0;

public:
	Socket(unsigned int port, std::string host = "");
	virtual ~Socket();

	virtual void Update(){}

	bool IsOpen();

	static void PotentialWait();

	friend class ViewController;
};