// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/utils/stringId.h>

namespace atta::script {

class Compiler {
  public:
    Compiler() = default;
    virtual ~Compiler() = default;

    virtual void compileAll() = 0;
    virtual void compileTarget(StringId target) = 0;

    std::vector<StringId> getTargets() const;
    std::map<StringId, std::vector<fs::path>> getTargetFiles() const { return _targetFiles; };

  protected:
    static std::vector<fs::path> getIncludedFiles(fs::path file);

    std::map<StringId, std::vector<fs::path>> _targetFiles;
};

} // namespace atta::script
