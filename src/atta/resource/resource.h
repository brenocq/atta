// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/utils/stringId.h>

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
