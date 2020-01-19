#include "Setting.h"



Setting::Setting()
{
	parent = nullptr;
}

void Setting::add(const std::string & key, const std::string value)
{
	if (settings.find(key) != settings.end()) //Already exists.
	{
		settings[key] = value;
	}
	else
	{
		settings.emplace(key, value);
	}
}

//无法显示专用化  居然跟调用的顺序有关 
template<>
std::string Setting::get<std::string>(const std::string& key) const
{
	//If this settings base has a setting value for it, look that up.
	if (settings.find(key) != settings.end())
	{
		return settings.at(key);
	}
	std::string st;
	return  st;
}

template<>
double Setting::get<double>(const std::string& key) const
{
	return atof(get<std::string>(key).c_str());
}