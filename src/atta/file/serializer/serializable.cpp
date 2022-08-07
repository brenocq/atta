//--------------------------------------------------
// Atta File Module
// serializable.cpp
// Date: 2021-11-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializable.h>

namespace atta::file {
unsigned Serializable::getSerializedSize(void* object) {
    std::ostringstream of;
    std::basic_ostream<char>::pos_type posBefore = of.tellp();
    serialize(of);
    ASSERT(of.tellp() != -1, "Failed to calculate componente serialized size")
    unsigned size = (int)of.tellp() - posBefore;
    return size;
}
} // namespace atta::file
