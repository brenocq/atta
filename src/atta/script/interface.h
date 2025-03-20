//--------------------------------------------------
// Atta Script Module
// interface.h
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_INTERFACE_H
#define ATTA_SCRIPT_INTERFACE_H

#include <atta/script/projectScript.h>
#include <atta/script/script.h>

namespace atta::script {

void startUp();
void shutDown();
void update(float dt);

Script* getScript(StringId target);
std::vector<StringId> getScriptSids();

ProjectScript* getProjectScript();
StringId getProjectScriptSid();

//----- Script Registration -----//
#ifdef ATTA_STATIC_PROJECT
ProjectScript* registerProjectScript(std::string name, ProjectScript* projectScript);
Script* registerScript(std::string name, Script* script);
#endif

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
    std::pair<const char*, atta::script::Script*> createScript() { return {#TYPE, static_cast<atta::script::Script*>(new TYPE())}; }                 \
    }
#endif

} // namespace atta::script

#endif // ATTA_SCRIPT_INTERFACE_H
