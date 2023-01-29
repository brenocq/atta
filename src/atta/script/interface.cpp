//--------------------------------------------------
// Atta Script Module
// manager.cpp
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/interface.h>
#include <atta/script/manager.h>

namespace atta::script {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }
void update(float dt) {
    PROFILE();
    Manager::getInstance().updateImpl(dt);
}

Script* getScript(StringId target) { return Manager::getInstance().getScriptImpl(target); }
std::vector<StringId> getScriptSids() { return Manager::getInstance().getScriptSidsImpl(); }

ProjectScript* getProjectScript() { return Manager::getInstance().getProjectScriptImpl(); }
StringId getProjectScriptSid() { return Manager::getInstance().getProjectScriptSidImpl(); }

#ifdef ATTA_STATIC_PROJECT
ProjectScript* registerProjectScript(std::string name, ProjectScript* projectScript) {
    Manager::getInstance()._projectScript.first = StringId(name);
    Manager::getInstance()._projectScript.second = projectScript;

    return projectScript;
}

Script* registerScript(std::string name, Script* script) {
    Manager::getInstance()._scripts[StringId(name)] = script;

    return script;
}
#endif

} // namespace atta::script
