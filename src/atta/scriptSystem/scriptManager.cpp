//--------------------------------------------------
// Atta Script System
// scriptManager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/scriptManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/scriptTargetEvent.h>

namespace atta
{
    ScriptManager& ScriptManager::getInstance()
    {
        static ScriptManager instance;
        return instance;
    }

    Script* ScriptManager::getScript(StringId target) { return getInstance().getScriptImpl(target); } 
    Script* ScriptManager::getScriptImpl(StringId target) const
    {
        if(_scripts.find(target) != _scripts.end())
            return _scripts.at(target);
        else
        {
            LOG_WARN("ScriptManager", "Trying to get script from target that does not exist: [w]$0[]", target);
            return nullptr;
        }
    }

    std::vector<StringId> ScriptManager::getScriptSids() { return getInstance().getScriptSidsImpl(); }
    std::vector<StringId> ScriptManager::getScriptSidsImpl() const
    {
        std::vector<StringId> scripts;
        for(auto [script, value] : _scripts)
            if(script != _projectScript.first)
                scripts.push_back(script);

        LOG_DEBUG("ScriptManager", "Scripts: $0", scripts);
        return scripts;
    }

    ProjectScript* ScriptManager::getProjectScript() { return getInstance().getProjectScriptImpl(); }
    ProjectScript* ScriptManager::getProjectScriptImpl() const
    {
        return _projectScript.second;
    }

    StringId ScriptManager::getProjectScriptSid() { return getInstance().getProjectScriptSidImpl(); }
    StringId ScriptManager::getProjectScriptSidImpl() const
    {
        return _projectScript.first;
    }
}

#ifdef ATTA_STATIC_PROJECT
#include <atta/scriptSystem/scriptManagerStatic.cpp>
#else
#include <atta/scriptSystem/scriptManagerDynamic.cpp>
#endif
