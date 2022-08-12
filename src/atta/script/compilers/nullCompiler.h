//--------------------------------------------------
// Atta Script Module
// nullCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H
#define ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H

#include <atta/script/compilers/compiler.h>

namespace atta::script {

class NullCompiler : public Compiler {
  public:
    NullCompiler() = default;

    void compileAll() override{};
    void compileTarget(StringId target) override{};
    void updateTargets() override{};
};

} // namespace atta::script

#endif // ATTA_SCRIPT_COMPILERS_NULL_COMPILER_H
