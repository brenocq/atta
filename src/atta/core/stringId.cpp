//--------------------------------------------------
// Atta String ID
// stringId.cpp
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/stringId.h>

namespace atta
{
	static std::unordered_map<hashId, std::string> stringIdTable;

	StringId::StringId(std::string str):
		_id(crc32b(str.c_str()))
	{
		stringIdTable[_id] = str;
	}

	const std::string& StringId::getString()
	{
		return stringIdTable[_id];
	}

	hashId StringId::getId()
	{
		return _id;	
	}
}
