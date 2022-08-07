//--------------------------------------------------
// Atta Script Module
// linuxLinker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_LINKERS_LINUX_LINKER_H
#define ATTA_SCRIPT_LINKERS_LINUX_LINKER_H
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
#endif // ATTA_SCRIPT_LINKERS_LINUX_LINKER_H
