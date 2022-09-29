//--------------------------------------------------
// Atta Script Module
// compiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_COMPILERS_COMPILER_H
#define ATTA_SCRIPT_COMPILERS_COMPILER_H

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

#endif // ATTA_SCRIPT_COMPILERS_COMPILER_H
