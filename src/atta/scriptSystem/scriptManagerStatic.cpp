//--------------------------------------------------
// Atta Script System
// scriptManagerStatic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/scripts.h>

namespace atta
{
    void ScriptManager::startUp() { getInstance().startUpImpl(); }
    void ScriptManager::startUpImpl()
    {
        // Publish registered scripts. Need to do here because scripts are 
        // registered before ComponentSystem::startUp())
        ScriptTargetEvent evt;
        evt.scriptSids = getScriptSids();
        EventManager::publish(evt);
    }

    void ScriptManager::shutDown() { getInstance().shutDownImpl(); }
    void ScriptManager::shutDownImpl()
    {
        if(_projectScript.second != nullptr)
            delete _projectScript.second;

        for(auto& [key, s] : _scripts)
            delete s;
        _scripts.clear();
    }

    ProjectScript* ScriptManager::registerProjectScript(std::string name, ProjectScript* projectScript)
    {
        LOG_VERBOSE("ScriptManager", "Registering project script [w]$0[]", name);
        getInstance()._projectScript.first = StringId(name);
        getInstance()._projectScript.second = projectScript;

        return projectScript;
    }

    Script* ScriptManager::registerScript(std::string name, Script* script)
    {
        LOG_VERBOSE("ScriptManager", "Registering script [w]$0[]", name);
        getInstance()._scripts[StringId(name)] = script;

        return script;
    }
}
