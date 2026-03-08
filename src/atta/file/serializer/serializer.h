// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
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
