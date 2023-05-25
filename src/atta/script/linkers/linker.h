//--------------------------------------------------
// Atta Script Module
// linker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_LINKERS_LINKER_H
#define ATTA_SCRIPT_LINKERS_LINKER_H

#include <atta/script/projectScript.h>
#include <atta/script/script.h>
#include <atta/utils/stringId.h>

namespace atta::script {

class Linker {
  public:
    Linker() = default;
    virtual ~Linker() = default;

    virtual void linkTarget(StringId target) = 0;
    virtual void releaseTarget(StringId target) = 0;
};

} // namespace atta::script

#endif // ATTA_SCRIPT_LINKERS_LINKER_H
