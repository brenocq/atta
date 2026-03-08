// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H
#define ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H

#include <atta/script/compilers/compiler.h>

namespace atta::script {

class NullCompiler : public Compiler {
  public:
    NullCompiler() = default;

    void compileAll() override {}
    void compileTarget(StringId target) override {}
};

} // namespace atta::script

#endif // ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H
