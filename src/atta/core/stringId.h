//--------------------------------------------------
// Atta String ID
// stringId.h
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_STRING_ID_H
#define ATTA_CORE_STRING_ID_H
#include <cstdint>

namespace atta
{
	using StringHash = std::uint32_t;

	class StringId
	{
	public:
		StringId(std::string str);

		const std::string& getString() const;
		uint32_t getId() const;

		bool operator==(StringHash sid) const;
		bool operator==(std::string str) const;
		bool operator==(StringId sid) const;

		static constexpr StringHash crc32b(const char* str);
	
	private:
		StringHash _id;
	};

	inline std::ostream& operator<<(std::ostream& os, const StringId& sid)
	{
		return os << sid.getString();
	}

	constexpr StringHash SID(const char* str)
	{
		return StringId::crc32b(str);
	}

	constexpr StringHash operator""_id(const char* str, std::size_t)
	{
		return StringId::crc32b(str);
	}
}

#include <atta/core/stringId.inl>
#endif// ATTA_CORE_STRING_ID_H
