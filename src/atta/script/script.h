//--------------------------------------------------
// Atta Script Module
// script.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPT_H
#define ATTA_SCRIPT_SCRIPT_H
#include <atta/component/interface.h>

namespace atta::script {

class Script {
  public:
    Script() = default;
    virtual ~Script(){};
    virtual void update(component::Entity entity, float dt) = 0;
};

#ifdef ATTA_STATIC_PROJECT
template <typename T>
class ScriptRegistration {
    static Script* reg;
};
#define ATTA_REGISTER_SCRIPT(TYPE)                                                                                                                   \
    template <>                                                                                                                                      \
    inline ::atta::script::Script* ::atta::script::ScriptRegistration<TYPE>::reg = ::atta::script::registerScript(#TYPE, new TYPE());
#else
#define ATTA_REGISTER_SCRIPT(TYPE)                                                                                                                   \
    extern "C" {                                                                                                                                     \
    std::pair<const char*, atta::script::Script*> createScript() { return {#TYPE, static_cast<atta::script::Script*>(new TYPE())}; }                                 \
    }
#endif

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPT_H
