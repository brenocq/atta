// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/script/projectScript.h>
#include <atta/script/script.h>
#include <atta/utils/stringId.h>

namespace atta::script {

class Linker {
  public:
    Linker() = default;
    virtual ~Linker() = default;

    virtual void linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) = 0;
    virtual void releaseTarget(StringId target) = 0;
};

} // namespace atta::script
