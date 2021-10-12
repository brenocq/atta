//--------------------------------------------------
// Atta Script System
// linuxLinker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
#define ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
#ifdef ATTA_OS_LINUX
#include <atta/scriptSystem/linkers/linker.h>

namespace atta
{
    class LinuxLinker : public Linker
    {
    public:
        LinuxLinker() = default;
        ~LinuxLinker() = default;

        void linkTarget(StringId target, Script** script, ProjectScript** projectScript) override;
        void releaseTarget(StringId target) override;

    private:
        std::unordered_map<StringId, void*> _targetHandles;
    };
}

#endif// ATTA_OS_LINUX
#endif// ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
