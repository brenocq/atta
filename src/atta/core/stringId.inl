//--------------------------------------------------
// Atta String ID
// stringId.inl
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    //---------- Class inline methods ----------//
    inline bool StringId::operator==(StringHash sid) const
    {
        return _id == sid;
    }

    inline bool StringId::operator==(std::string str) const
    {
        return getString() == str;
    }

    inline bool StringId::operator==(StringId sid) const
    {
        return _id == sid._id;
    }

    inline bool StringId::operator<(StringId other) const
    {
        return _id < other._id;
    }


    //---------- Extern inline functions ----------//
    inline std::ostream& operator<<(std::ostream& os, const StringId& sid)
    {
        return os << sid.getString();
    }

    constexpr StringHash SID(const char* str)
    {
        return StringId::crc32b(str);
    }

    constexpr StringHash operator""_sid(const char* str, std::size_t)
    {
        return SID(str);
    }

    constexpr StringHash StringId::crc32b(const char* str)
    {
        // Reference: Book Hacker's Delight 2nd Edition
        const char* message = str;
        int i = 0, j = 0;
        StringHash byte = 0, crc = 0xFFFFFFFF, mask = 0;

        while(message[i] != 0)
        {
            // Get next byte
            byte = message[i];
            crc = crc ^ byte;
            for(j = 7; j >= 0; j--)
            {
                // Do eight times
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
            i = i + 1;
        }
        return ~crc;
    }
}
