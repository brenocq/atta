//--------------------------------------------------
// Atta Script System
// compiler.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/compilers/compiler.h>

namespace atta
{
    std::vector<StringId> Compiler::getTargets() const
    {
        std::vector<StringId> targets;
        targets.clear();

        for(auto& target : _targetFiles)
            targets.push_back(target.first);

        return targets;
    }
}
