#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>

#include <cctype>
#include <fstream>
#include <stdio.h>
#include <sstream> // ostringstream
#include <regex> // regex parsing for temp flow graph
#include <string> //Parsing strings (stod, stoul).

class Setting
{
public:
	Setting();
	void add(const std::string& key,const std::string value);

	template<typename A> 
	A get(const std::string& key) const;

private:
	Setting* parent;

	std::unordered_map<std::string, std::string> settings;
};
