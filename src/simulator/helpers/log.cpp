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

std::string Log::toString(atta::mat4 mat)
{
	std::string res = "\n[[" + std::to_string(mat.data[0]) + ", " + std::to_string(mat.data[1]) + ", "
								+ std::to_string(mat.data[2]) + ", " + std::to_string(mat.data[3]) + "],";

	res += "\n [" + std::to_string(mat.data[4]) + ", " + std::to_string(mat.data[5]) + ", "
								+ std::to_string(mat.data[6]) + ", " + std::to_string(mat.data[7]) + "],";

	res += "\n [" + std::to_string(mat.data[8]) + ", " + std::to_string(mat.data[9]) + ", "
								+ std::to_string(mat.data[10]) + ", " + std::to_string(mat.data[11]) + "],";
	res += "\n [" + std::to_string(mat.data[12]) + ", " + std::to_string(mat.data[13]) + ", "

								+ std::to_string(mat.data[14]) + ", " + std::to_string(mat.data[15]) + "]]\n";
	return res;
}
