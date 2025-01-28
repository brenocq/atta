//--------------------------------------------------
// Atta File Module
// serializer.h
// Date: 2025-01-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SERIALIZER_SERIALIZER_H
#define ATTA_FILE_SERIALIZER_SERIALIZER_H
#include <atta/file/serializer/section.h>

namespace atta::file {

class Serializer {
  public:
    void serialize(const fs::path& file);
    void deserialize(const fs::path& file);

  private:
    std::vector<Section> _sections;
    fs::path _file;
};

} // namespace atta::file

#endif // ATTA_FILE_SERIALIZER_SERIALIZER_H
