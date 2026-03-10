// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/resource/resource.h>

namespace atta::resource {

Resource::Resource(const fs::path filename) : _id(StringId(filename.string())), _filename(filename), _data(nullptr) {}

} // namespace atta::resource
