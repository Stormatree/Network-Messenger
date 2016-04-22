#pragma once

#include <string>
#include <unordered_map>

class Socket;

class ViewController{
public:
	using DataMap = std::unordered_map<unsigned int, std::string>;

private:
	Socket& const _socket;
	const std::string _key;

	DataMap _data;

protected:
	DataMap _amendments;

	bool _DataExists(unsigned int id);

	std::string _GetData(unsigned int id);
	void _SetAmendment(unsigned int id, std::string amendment);
	void _SetData(unsigned int id, std::string amendment);

	void _SendAmendments(int exclude = -1, int exclusive = -1);
	void _SendMessage(std::string message, int exclude = -1, int exclusive = -1, std::string key = "");

	const DataMap& _GetDataMap() const;

public:
	ViewController(std::string key, Socket* socket);
	virtual ~ViewController();

	const DataMap& GetAmendmentsMap() const;
	const DataMap& GetDataMap() const;

	virtual void OnData(const DataMap& data, int client = -1);
	virtual void OnMessage(std::string message, int client = -1){};

	std::string Key() const;

	friend class Socket;
};