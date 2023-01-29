//--------------------------------------------------
// Atta Script Module
// linuxCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_COMPILERS_LINUX_COMPILER_H
#define ATTA_SCRIPT_COMPILERS_LINUX_COMPILER_H
#ifdef ATTA_OS_LINUX

#include <atta/script/compilers/compiler.h>

namespace atta::script {

class LinuxCompiler : public Compiler {
  public:
    LinuxCompiler();
    ~LinuxCompiler();

    void compileAll() override;
    void compileTarget(StringId target) override;

  private:
    void updateTargets();
    void findTargetFiles(StringId target);
    std::string runCommand(std::string cmd, bool print = false, bool keepColors = false);

    std::string _compiler;
};

} // namespace atta::script

#endif // ATTA_OS_LINUX
#endif // ATTA_SCRIPT_COMPILERS_LINUX_COMPILER_H
