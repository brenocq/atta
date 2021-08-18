//----------------------------------------
// Atta Log
// log.cpp
// Date: 2021-08-16
// By Breno Cunha Queiroz
//----------------------------------------
#include <atta/core/log.h>

namespace atta
{

	void Log::replace(std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		std::string::size_type pos = 0u;
		while((pos = str.find(oldStr, pos)) != std::string::npos)
		{
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
	}

	std::string Log::toString(const char* str)
	{
		return std::string(str);
	}

	std::string Log::toString(std::string str)
	{
		return str;
	}

}
