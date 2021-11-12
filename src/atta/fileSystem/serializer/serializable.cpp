//--------------------------------------------------
// Atta File System
// serializable.cpp
// Date: 2021-11-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/serializer/serializable.h>

namespace atta
{
    unsigned Serializable::getSerializedSize(void* object, size_t maxSerializedSize)
    {
        std::vector<char> buffer(maxSerializedSize);
        std::ostringstream of;
        std::basic_ostream<char>::pos_type posBefore = of.tellp();
        serialize(of);
        ASSERT(of.tellp() != -1, "Failed to calculate componente serialized size")
        unsigned size = (int)of.tellp() - posBefore;
        return size;
    }
}
