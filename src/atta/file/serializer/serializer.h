//--------------------------------------------------
// Atta File Module
// serializer.h
// Date: 2021-11-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SERIALIZER_SERIALIZER_H
#define ATTA_FILE_SERIALIZER_SERIALIZER_H
#include <atta/utils/stringId.h>

namespace atta::file {

// Helpers to write binary data
template <typename T>
inline void write(std::ostream& os, T x);
template <typename T, size_t N>
inline void write(std::ostream& os, T (&x)[N]);
template <typename T>
inline void write(std::ostream& os, T* x, size_t size);

// Helpers to read binary data
template <typename T>
inline void read(std::istream& is, T& x);
template <typename T, size_t N>
inline void read(std::istream& is, T (&x)[N]);
template <typename T>
inline void read(std::istream& is, T* x, size_t size);

} // namespace atta::file

#include <atta/file/serializer/serializer.inl>
#endif // ATTA_FILE_SERIALIZER_SERIALIZER_H
