#include "Encoder.hpp"

#include <algorithm>

std::string Encoder::EncodeMessage(std::string key, std::string message){
	if (message == "")
		return "";

	return "m:" + key + "(" + message + ";)";
}

std::string Encoder::EncodeAmendments(const ViewController* vc, bool useData){
	std::string data = "a:" + vc->Key() + "(";

	if (useData){
		if (!vc->GetDataMap().size())
			return "";

		for (auto i : vc->GetDataMap())
			data += std::to_string(i.first) + ":" + i.second + ";";
	}
	else{
		if (!vc->GetAmendmentsMap().size())
			return "";

		for (auto i : vc->GetAmendmentsMap())
			data += std::to_string(i.first) + ":" + i.second + ";";
	}

	data += ")";

	return data;
}

void Encoder::DecodeKeyType(std::string data, std::string & key, unsigned int & type){
	if (data.length() > 0){
		if (data.at(0) == 'm'){
			type = MESSAGE;
		}
		else if (data.at(0) == 'a'){
			type = AMENDMENT;
		}
	}
	else{
		type = ERROR;
		key = "";
	}

	size_t openingBracket = data.find_first_of("(");

	key = data.substr(2, openingBracket - 2);

	size_t closingBracket = data.find_first_of(")");
}

void Encoder::DecodeAmendments(std::string data,ViewController::DataMap& map){
	size_t openingBracket = data.find_first_of("(");

	std::string contents = data.substr(openingBracket + 1, data.find_first_of(")") - openingBracket - 1);
	
	while (contents.find(";") != std::string::npos){
		contents = contents.substr(0, contents.find_last_of(";"));

		size_t colon = contents.find_last_of(":");

		std::string value = contents.substr(colon + 1);

		size_t semicolon = contents.find_last_of(";");

		std::string id = contents.substr(semicolon + 1, colon - semicolon - 1);

		map[std::stoi(id)] = value;
	}
}

void Encoder::DecodeMessage(std::string data, std::string & message){
	size_t openingBracket = data.find_first_of("(");

	std::string contents = data.substr(openingBracket + 1, data.find_first_of(";") - openingBracket - 1);

	message = contents;
}

bool Encoder::SplitSections(std::string data, std::string & first, std::string & second){
	size_t firstBracket = data.find_first_of(")");
	size_t lastBracket = data.find_last_of(")");

	if (firstBracket == lastBracket)
		return false;
	
	first = data.substr(0, firstBracket + 1);
	second = data.substr(firstBracket + 1, lastBracket - firstBracket);

	return true;
}