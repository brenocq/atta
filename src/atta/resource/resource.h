//--------------------------------------------------
// Atta Resource Module
// resource.h
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCE_H
#define ATTA_RESOURCE_RESOURCE_H
#include <atta/core/stringId.h>

namespace atta::resource {
class Resource {
  public:
    Resource(const fs::path filename);
    virtual ~Resource() = default;

    StringId getId() const { return _id; }
    uint8_t* getData() const { return _data; }

  protected:
    StringId _id;
    fs::path _filename;
    uint8_t* _data;
};
} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCE_H
