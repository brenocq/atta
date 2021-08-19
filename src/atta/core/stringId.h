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
	using hashId = std::uint32_t;

	class StringId
	{
		public:
			StringId(std::string str);

			const std::string& getString();
			uint32_t getId();

			static constexpr hashId crc32b(const char* str);
		
		private:
			hashId _id;
	};

	constexpr hashId SID(const char* str)
	{
		return StringId::crc32b(str);
	}

	constexpr hashId operator""_id(const char* str, std::size_t)
	{
		return StringId::crc32b(str);
	}
}

#include <atta/core/stringId.inl>
#endif// ATTA_CORE_STRING_ID_H
