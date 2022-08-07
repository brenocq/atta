//--------------------------------------------------
// Atta Script Module
// manager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/scriptTarget.h>
#include <atta/event/manager.h>
#include <atta/script/manager.h>

namespace atta::script {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

Script* Manager::getScript(StringId target) { return getInstance().getScriptImpl(target); }
Script* Manager::getScriptImpl(StringId target) const {
    if (_scripts.find(target) != _scripts.end())
        return _scripts.at(target);
    else {
        LOG_WARN("script::Manager", "Trying to get script from target that does not exist: [w]$0[]", target);
        return nullptr;
    }
}

std::vector<StringId> Manager::getScriptSids() { return getInstance().getScriptSidsImpl(); }
std::vector<StringId> Manager::getScriptSidsImpl() const {
    std::vector<StringId> scripts;
    for (auto [script, value] : _scripts)
        if (script != _projectScript.first)
            scripts.push_back(script);
    return scripts;
}

ProjectScript* Manager::getProjectScript() { return getInstance().getProjectScriptImpl(); }
ProjectScript* Manager::getProjectScriptImpl() const { return _projectScript.second; }

StringId Manager::getProjectScriptSid() { return getInstance().getProjectScriptSidImpl(); }
StringId Manager::getProjectScriptSidImpl() const { return _projectScript.first; }

} // namespace atta::script

#ifdef ATTA_STATIC_PROJECT
#include <atta/script/managerStatic.cpp>
#else
#include <atta/script/managerDynamic.cpp>
#endif
