//--------------------------------------------------
// Atta Script Module
// projectScript.h
// Date: 2021-09-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_PROJECT_SCRIPT_H
#define ATTA_SCRIPT_PROJECT_SCRIPT_H

namespace atta::script {

class ProjectScript {
  public:
    ProjectScript() = default;
    virtual ~ProjectScript() {};

    //---------- Load/Unload ----------//
    virtual void onLoad() {};
    virtual void onUnload() {};

    //---------- Simulation ----------//
    virtual void onStart() {};
    virtual void onStop() {};

    virtual void onContinue() {};
    virtual void onPause() {};

    virtual void onUpdateBefore(float delta) {};
    virtual void onUpdateAfter(float delta) {};

    //---------- Editor ----------//
    virtual void onUIRender() {};

    //---------- While ----------//
    virtual void onAttaLoop() {};
};
} // namespace atta::script

#ifdef ATTA_STATIC_PROJECT
#include <atta/script/interface.h>
namespace atta::script {
template <typename T>
class ProjectScriptRegistration {
    static ProjectScript* reg;
};
} // namespace atta::script
#define ATTA_REGISTER_PROJECT_SCRIPT(TYPE)                                                                                                           \
    template <>                                                                                                                                      \
    inline ::atta::script::ProjectScript* ::atta::script::ProjectScriptRegistration<TYPE>::reg =                                                     \
        ::atta::script::registerProjectScript(#TYPE, new TYPE());

#else

#define ATTA_REGISTER_PROJECT_SCRIPT(TYPE)                                                                                                           \
    extern "C" {                                                                                                                                     \
    std::pair<const char*, atta::script::ProjectScript*> createProjectScript() {                                                                     \
        return {#TYPE, static_cast<atta::script::ProjectScript*>(new TYPE())};                                                                       \
    }                                                                                                                                                \
    }
#endif

#endif // ATTA_SCRIPT_PROJECT_SCRIPT_H
