//--------------------------------------------------
// Atta Script System
// scriptManager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/scriptManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/fileWatchEvent.h>
#include <atta/eventSystem/events/projectBeforeDeserializeEvent.h>
#include <atta/eventSystem/events/projectOpenEvent.h>
#include <atta/eventSystem/events/projectCloseEvent.h>
#include <atta/eventSystem/events/scriptTargetEvent.h>
#include <atta/scriptSystem/compilers/nullCompiler.h>
#include <atta/scriptSystem/compilers/linuxCompiler.h>
#include <atta/scriptSystem/linkers/nullLinker.h>
#include <atta/scriptSystem/linkers/linuxLinker.h>

#include <atta/componentSystem/componentManager.h>

namespace atta
{
    ScriptManager& ScriptManager::getInstance()
    {
        static ScriptManager instance;
        return instance;
    }

    void ScriptManager::startUp() { getInstance().startUpImpl(); }
    void ScriptManager::startUpImpl()
    {
#ifdef ATTA_OS_LINUX
        _compiler = std::static_pointer_cast<Compiler>(std::make_shared<LinuxCompiler>());
        _linker = std::static_pointer_cast<Linker>(std::make_shared<LinuxLinker>());
#else
        _compiler = std::static_pointer_cast<Compiler>(std::make_shared<NullCompiler>());
        _linker = std::static_pointer_cast<Linker>(std::make_shared<NullLinker>());
#endif

        EventManager::subscribe<FileWatchEvent>(BIND_EVENT_FUNC(ScriptManager::onFileChange));
        EventManager::subscribe<ProjectBeforeDeserializeEvent>(BIND_EVENT_FUNC(ScriptManager::onProjectOpen));
        EventManager::subscribe<ProjectOpenEvent>(BIND_EVENT_FUNC(ScriptManager::onProjectOpen));
        EventManager::subscribe<ProjectCloseEvent>(BIND_EVENT_FUNC(ScriptManager::onProjectClose));

        _projectScript = std::make_pair(StringId(), nullptr);
    }

    void ScriptManager::shutDown() { getInstance().shutDownImpl(); }
    void ScriptManager::shutDownImpl()
    {

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

        //LOG_DEBUG("ScriptManager", "Scripts: $0", scripts);
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

    void ScriptManager::onFileChange(Event& event)
    {
        FileWatchEvent& e = reinterpret_cast<FileWatchEvent&>(event);

        if(e.file.filename() == "CMakeLists.txt")
        {
            updateAllTargets();
            return;
        }

        for(auto target : _compiler->getTargetFiles())
            for(auto file : target.second)
                if(file == e.file)
                    updateTarget(target.first);

        // Publish event
        ScriptTargetEvent evt;
        evt.scriptSids = getScriptSids();
        EventManager::publish(evt);
    }

    void ScriptManager::onProjectOpen(Event& event)
    {
        updateAllTargets();

        // Publish event
        ScriptTargetEvent evt;
        evt.scriptSids = getScriptSids();
        EventManager::publish(evt);
    }

    void ScriptManager::onProjectClose(Event& event)
    {
        // Release all targets
        for(auto target : _compiler->getTargets())
            releaseTarget(target);

        // Publish event
        ScriptTargetEvent evt;
        evt.scriptSids = getScriptSids();
        EventManager::publish(evt);
    }

    void ScriptManager::updateAllTargets()
    {
        // Release all targets
        for(auto target : _compiler->getTargets())
            releaseTarget(target);

        // Recompile all targets
        _compiler->compileAll();
        _compiler->updateTargets();

        // Link each target in the project
        for(auto target : _compiler->getTargets())
            linkTarget(target);

        //LOG_DEBUG("ScriptManager", "Targets updated: $0", _compiler->getTargets());
    }

    void ScriptManager::updateTarget(StringId target)
    {
        // Delete all scripts related to this target
        releaseTarget(target);

        // Compile and link specific target
        _compiler->compileTarget(target);

        // Link target
        linkTarget(target);
    }

    void ScriptManager::linkTarget(StringId target)
    {
        Script* script = nullptr;
        ProjectScript* projectScript = nullptr;
        _linker->linkTarget(target, &script, &projectScript);
        if(script != nullptr)
            _scripts[target] = script;

        if(projectScript != nullptr)
        {
            if(_projectScript.second != nullptr)
                LOG_WARN("ScriptManager", "Multiple project scripts found. There must be only one project script in the project. Found at [w]$0[] and [w]$1[]", _projectScript.first, target);
            _projectScript.first = target;
            _projectScript.second = projectScript;

            _projectScript.second->onLoad();
        }

        if(script == nullptr && projectScript == nullptr)
            LOG_WARN("ScriptManager", "Target $0 does not have any script", target);
    }

    void ScriptManager::releaseTarget(StringId target)
    {
        // Delete script
        if(_scripts.find(target) != _scripts.end())
            delete _scripts[target];
        _scripts.erase(target);

        // Delete project script
        if(_projectScript.first == target)
        {
            _projectScript.second->onUnload();
            //ComponentManager::unregisterCustomComponents();

            _projectScript.first = StringId();
            delete _projectScript.second;
            _projectScript.second = nullptr;
        }

        // Release target
        _linker->releaseTarget(target);
    }
}
