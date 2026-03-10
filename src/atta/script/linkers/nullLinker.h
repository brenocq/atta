// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/script/linkers/linker.h>

namespace atta::script {

class NullLinker : public Linker {
  public:
    NullLinker() = default;
    ~NullLinker() = default;

    void linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) override;
    virtual void releaseTarget(StringId target) override {}
};

} // namespace atta::script
