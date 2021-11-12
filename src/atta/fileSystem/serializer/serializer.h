//--------------------------------------------------
// Atta File System
// serializer.h
// Date: 2021-11-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_SERIALIZER_SERIALIZER_H
#define ATTA_FILE_SYSTEM_SERIALIZER_SERIALIZER_H
#include <atta/core/stringId.h>

namespace atta
{
    // Helpers to write binary data
    template <typename T>
    inline void write(std::ostream& os, T x);
    template<typename T, size_t N>
    inline void write(std::ostream& os, T(&x)[N]);
    template <typename It>
    inline void write(std::ostream& os, It begin, It end);

    // Helpers to read binary data
    template <typename T>
    inline void read(std::istream& is, T& x);
    template<typename T, size_t N>
    inline void read(std::istream& is, T(&x)[N]);
}

#include <atta/fileSystem/serializer/serializer.inl>
#endif// ATTA_FILE_SYSTEM_SERIALIZER_SERIALIZER_H
