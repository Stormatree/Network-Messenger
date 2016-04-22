#pragma once

#include "ViewController.hpp"

class Encoder{
public:
	enum Type{
		ERROR,
		MESSAGE,
		AMENDMENT
	};

	static std::string EncodeMessage(std::string key, std::string message);

	static std::string EncodeAmendments(const ViewController* vc, bool useData = false);

	static void DecodeAmendments(std::string data, ViewController::DataMap& map);

	static void DecodeMessage(std::string data, std::string& message);

	static void DecodeKeyType(std::string data, std::string& key, unsigned int& type);

	static bool SplitSections(std::string data, std::string& first, std::string& second);
};