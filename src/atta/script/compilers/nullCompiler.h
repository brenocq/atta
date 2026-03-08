// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/script/compilers/compiler.h>

namespace atta::script {

class NullCompiler : public Compiler {
  public:
    NullCompiler() = default;

    void compileAll() override {}
    void compileTarget(StringId target) override {}
};

} // namespace atta::script
