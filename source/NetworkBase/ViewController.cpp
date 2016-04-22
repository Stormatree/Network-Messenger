#include "ViewController.hpp"

#include "Socket.hpp"

bool ViewController::_DataExists(unsigned int id){
	if (_data.find(id) != _data.end())
		return true;
	
	return false;
}

std::string ViewController::_GetData(unsigned int id){
	auto data = _data.find(id);

	if (data != _data.end()){
		if (typeid(std::string) == typeid(std::string))
			return data->second;
	}

	return "";
}

void ViewController::_SetAmendment(unsigned int id, std::string amendment){
	_amendments[id] = amendment;

	for (auto i : _socket._GetViews(_key))
		i->_amendments[id] = amendment;
}

void ViewController::_SetData(unsigned int id, std::string amendment){
	_data[id] = amendment;

	for (auto i : _socket._GetViews(_key))
		i->_data[id] = amendment;
}

void ViewController::_SendAmendments(int exclude, int exclusive){
	_socket._Send(this, exclude, exclusive);
}

void ViewController::_SendMessage(std::string message, int exclude, int exclusive, std::string key){
	if (key == "")
		key = _key;

	_socket._Message(key, message, exclude, exclusive);
}

const ViewController::DataMap & ViewController::_GetDataMap() const{
	return _data;
}

ViewController::ViewController(std::string key, Socket* socket) : _key(key), _socket(*socket){
	_socket._AddView(this);
}

ViewController::~ViewController(){
	//_socket._RemoveView(this);
}

const ViewController::DataMap& ViewController::GetAmendmentsMap() const{
	return _amendments;
}
 
const ViewController::DataMap& ViewController::GetDataMap() const{
	return _data;
}


void ViewController::OnData(const ViewController::DataMap& data, int client){
	for (auto i : data)
		_data[i.first] = i.second;
}

std::string ViewController::Key() const{
	return _key;
}
