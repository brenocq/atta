//--------------------------------------------------
// Atta String ID
// stringId.cpp
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/stringId.h>

namespace atta
{
	static std::unordered_map<StringHash, std::string> stringIdTable;

	StringId::StringId():
		_id(crc32b("emptyStringId"))
	{
		std::string str = "emptyStringId";

		DASSERT(!(stringIdTable.find(_id) != stringIdTable.end() &&
			stringIdTable[_id] != str), "String id hash collisiong between [w]$0[] and [w]$1[] ($2)", stringIdTable[_id], str, _id);

		stringIdTable[_id] = str;
	}

	StringId::StringId(std::string str):
		_id(crc32b(str.c_str()))
	{
		DASSERT(!(stringIdTable.find(_id) != stringIdTable.end() &&
			stringIdTable[_id] != str), "String id hash collisiong between [w]$0[] and [w]$1[] ($2)", stringIdTable[_id], str, _id);

		stringIdTable[_id] = str;
	}

	StringId::StringId(StringHash id):
		_id(id)
	{
		DASSERT(stringIdTable.find(id) != stringIdTable.end(), "Can not create StringId from StringHash ($0) that was never registered", id);
	}

	const std::string& StringId::getString() const
	{
		return stringIdTable[_id];
	}

	StringHash StringId::getId() const
	{
		return _id;	
	}
	
	std::vector<std::string> StringId::getStrings()
	{
		std::vector<std::string> strings;
		for(auto str : stringIdTable)
			strings.push_back(str.second);

		return strings;
	}

#ifdef ATTA_DEBUG_BUILD
	StringHash SSID(const char* str)
	{
		return StringId(str).getId();
	}
#endif
}
