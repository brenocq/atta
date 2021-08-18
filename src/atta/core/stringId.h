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

			static constexpr hashId crc32b(const char* str)
			{
				// Reference: Book Hacker's Delight 2nd Edition
				const unsigned char* message = (const unsigned char*)(str);
				int i = 0, j = 0;
				hashId byte = 0, crc = 0xFFFFFFFF, mask = 0;

				while(message[i] != 0)
				{
					// Get next byte.
					byte = message[i];            
					crc = crc ^ byte;
					for(j = 7; j >= 0; j--)
					{
						// Do eight times.
						mask = -(crc & 1);
						crc = (crc >> 1) ^ (0xEDB88320 & mask);
					}
					i = i + 1;
				}
				return ~crc;
			}
		
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

#endif// ATTA_CORE_STRING_ID_H
