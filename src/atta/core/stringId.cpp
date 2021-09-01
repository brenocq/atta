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

	StringId::StringId(std::string str):
		_id(crc32b(str.c_str()))
	{
		stringIdTable[_id] = str;
	}

	const std::string& StringId::getString() const
	{
		return stringIdTable[_id];
	}

	StringHash StringId::getId() const
	{
		return _id;	
	}

	bool StringId::operator==(StringHash sid) const
	{
		return _id == sid;
	}

	bool StringId::operator==(std::string str) const
	{
		return getString() == str;
	}

	bool StringId::operator==(StringId sid) const
	{
		return _id == sid.getId();
	}
}
