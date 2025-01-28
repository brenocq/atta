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
    // File-based serialization
    void serialize(const fs::path& file);
    void deserialize(const fs::path& file);

    // String-based serialization
    std::string toString() const;
    void fromString(const std::string& data);

    const std::vector<Section>& getSections() const { return _sections; }
    void addSection(const Section& section) { _sections.push_back(section); }

  private:
    std::vector<Section> _sections;
    fs::path _file;
};

} // namespace atta::file

#endif // ATTA_FILE_SERIALIZER_SERIALIZER_H
