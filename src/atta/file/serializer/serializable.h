//--------------------------------------------------
// Atta File Module
// serializable.h
// Date: 2021-11-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SERIALIZER_SERIALIZABLE_H
#define ATTA_FILE_SERIALIZER_SERIALIZABLE_H

namespace atta
{
    class Serializable
    {
    public:
        virtual void serialize(std::ostream& os) = 0;
        virtual void deserialize(std::istream& is) = 0;
        unsigned getSerializedSize(void* object);
    };
}

#endif// ATTA_FILE_SERIALIZER_SERIALIZABLE_H
