//--------------------------------------------------
// Atta Script System
// linker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H
#define ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H
#include <atta/scriptSystem/script.h>
#include <atta/scriptSystem/projectScript.h>
#include <atta/core/stringId.h>

namespace atta
{
    class Linker
    {
    public:
        Linker() = default;
        virtual ~Linker() = default;

        virtual void linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) = 0;
        virtual void releaseTarget(StringId target) = 0;
    };
}

#endif// ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H
