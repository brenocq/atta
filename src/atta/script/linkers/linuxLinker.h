// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#ifdef ATTA_OS_LINUX

#include <atta/script/linkers/linker.h>

namespace atta::script {

class LinuxLinker : public Linker {
  public:
    LinuxLinker() = default;
    ~LinuxLinker() = default;

    void linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) override;
    void releaseTarget(StringId target) override;

  private:
    std::unordered_map<StringId, void*> _targetHandles;
};

} // namespace atta::script

#endif // ATTA_OS_LINUX
