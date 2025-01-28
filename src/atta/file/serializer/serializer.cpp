//--------------------------------------------------
// Atta File Module
// serializer.cpp
// Date: 2025-01-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>

namespace atta::file {

void Serializer::serialize(const fs::path& file) { LOG_DEBUG("Serializer", "Serializing to [w]$0[]", file); }

void Serializer::deserialize(const fs::path& file) { LOG_DEBUG("Serializer", "Deserializing from [w]$0[]", file); }

} // namespace atta::file
