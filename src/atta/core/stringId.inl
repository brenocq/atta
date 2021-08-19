//--------------------------------------------------
// Atta String ID
// stringId.inl
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	constexpr hashId StringId::crc32b(const char* str)
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
}
