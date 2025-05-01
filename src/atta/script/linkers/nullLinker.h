//--------------------------------------------------
// Atta Script Module
// nullLinker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_LINKERS_NULL_LINKER_H
#define ATTA_SCRIPT_LINKERS_NULL_LINKER_H
#include <atta/script/linkers/linker.h>

namespace atta::script {

class NullLinker : public Linker {
  public:
    NullLinker() = default;
    ~NullLinker() = default;

    void linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) override;
    virtual void releaseTarget(StringId target) override {};
};

} // namespace atta::script

#endif // ATTA_SCRIPT_LINKERS_NULL_LINKER_H
