//----------------------------------------
// Atta Log
// log.cpp
// Date: 2020-08-20
// By Breno Cunha Queiroz
//----------------------------------------
#include "log.h"
#include "log.h"
#include <stdio.h>
#include <iostream>
#include "defines.h"

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
