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
        StringId();
        StringId(std::string str);
        StringId(const char* str);
        StringId(StringHash id);

        const std::string& getString() const;
        uint32_t getId() const;

        inline bool operator==(StringHash sid) const;
        inline bool operator==(std::string str) const;
        inline bool operator==(const char* str) const;
        inline bool operator==(StringId sid) const;
        inline bool operator!=(StringHash sid) const;
        inline bool operator!=(std::string str) const;
        inline bool operator!=(StringId sid) const;

        inline bool operator<(StringId other) const;
        inline bool operator>(StringId other) const;

        static constexpr StringHash crc32b(const char* str);
        static std::vector<std::string> getStrings();

    private:
        StringHash _id;
    };

    inline std::ostream& operator<<(std::ostream& os, const StringId& sid);

    //----- (S)tring (ID) -----
    // Convert at compile-time the string to its hash
    // Can be used with templates, constexpr, switch case, etc
    constexpr StringHash SID(const char* str);
    constexpr StringHash operator""_sid(const char* str, std::size_t);
    //----- (S)aved (S)tring (ID) -----
    // SSID is compiled like SID if not compiling as debug
    // When compiling as debug, SSID executes at runtime and add 
    // an entry to the stringId table (useful for debugging)
#ifdef ATTA_DEBUG_BUILD
    StringHash SSID(const char* str);
    StringHash operator""_ssid(const char* str, std::size_t);
#else
    constexpr auto SSID = SID;
    constexpr StringHash operator""_ssid(const char* str, std::size_t)
    {
        return SSID(str);
    }
#endif
}

namespace std {
    template<> struct hash<atta::StringId> {
        size_t operator()(atta::StringId const& sid) const {
            return static_cast<size_t>(sid.getId());
        }
    };
}


#include <atta/core/stringId.inl>
#endif// ATTA_CORE_STRING_ID_H
