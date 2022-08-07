//--------------------------------------------------
// Atta Script Module
// managerDynamic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentManager.h>
#include <atta/event/events/fileWatchEvent.h>
#include <atta/event/events/projectBeforeDeserializeEvent.h>
#include <atta/event/events/projectCloseEvent.h>
#include <atta/event/events/projectOpenEvent.h>
#include <atta/script/compilers/linuxCompiler.h>
#include <atta/script/compilers/nullCompiler.h>
#include <atta/script/linkers/linuxLinker.h>
#include <atta/script/linkers/nullLinker.h>

namespace atta::script {
void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
#ifdef ATTA_OS_LINUX
    _compiler = std::static_pointer_cast<Compiler>(std::make_shared<LinuxCompiler>());
    _linker = std::static_pointer_cast<Linker>(std::make_shared<LinuxLinker>());
#else
    _compiler = std::static_pointer_cast<Compiler>(std::make_shared<NullCompiler>());
    _linker = std::static_pointer_cast<Linker>(std::make_shared<NullLinker>());
#endif

    EventManager::subscribe<FileWatchEvent>(BIND_EVENT_FUNC(Manager::onFileChange));
    EventManager::subscribe<ProjectBeforeDeserializeEvent>(BIND_EVENT_FUNC(Manager::onProjectOpen));
    EventManager::subscribe<ProjectOpenEvent>(BIND_EVENT_FUNC(Manager::onProjectOpen));
    EventManager::subscribe<ProjectCloseEvent>(BIND_EVENT_FUNC(Manager::onProjectClose));

    _projectScript = std::make_pair(StringId(), nullptr);
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {}

void Manager::onFileChange(Event& event) {
    FileWatchEvent& e = reinterpret_cast<FileWatchEvent&>(event);

    if (e.file.filename() == "CMakeLists.txt") {
        updateAllTargets();
        return;
    }

    for (auto target : _compiler->getTargetFiles())
        for (auto file : target.second)
            if (file == e.file)
                updateTarget(target.first);

    // Publish event
    ScriptTargetEvent evt;
    evt.scriptSids = getScriptSids();
    EventManager::publish(evt);
}

void Manager::onProjectOpen(Event& event) {
    updateAllTargets();

    // Publish event
    ScriptTargetEvent evt;
    evt.scriptSids = getScriptSids();
    EventManager::publish(evt);
}

void Manager::onProjectClose(Event& event) {
    // Release all targets
    for (auto target : _compiler->getTargets())
        releaseTarget(target);

    // Publish event
    ScriptTargetEvent evt;
    evt.scriptSids = getScriptSids();
    EventManager::publish(evt);
}

void Manager::updateAllTargets() {
    // Release all targets
    for (auto target : _compiler->getTargets())
        releaseTarget(target);

    // Recompile all targets
    _compiler->compileAll();
    _compiler->updateTargets();

    // Link each target in the project
    for (auto target : _compiler->getTargets())
        linkTarget(target);
    // LOG_DEBUG("Manager", "Targets updated: $0", _compiler->getTargets());
}

void Manager::updateTarget(StringId target) {
    // Delete all scripts related to this target
    releaseTarget(target);

    // Compile and link specific target
    _compiler->compileTarget(target);

    // Link target
    linkTarget(target);
}

void Manager::linkTarget(StringId target) {
    Script* script = nullptr;
    ProjectScript* projectScript = nullptr;
    std::string name;
    _linker->linkTarget(target, &script, &projectScript, name);
    if (script != nullptr)
        _scripts[StringId(name)] = script;

    if (projectScript != nullptr) {
        if (_projectScript.second != nullptr)
            LOG_WARN("script::Manager",
                     "Multiple project scripts found. There must be only one project script in the project. Found at [w]$0[] and [w]$1[]",
                     _projectScript.first, target);

        _projectScript.first = StringId(name);
        _projectScript.second = projectScript;

        _projectScript.second->onLoad();
    }

    if (script || projectScript)
        _targetToScript[target] = StringId(name);
}

void Manager::releaseTarget(StringId target) {
    // Delete script
    if (_scripts.find(_targetToScript[target]) != _scripts.end())
        delete _scripts[_targetToScript[target]];
    _scripts.erase(_targetToScript[target]);

    // Delete project script
    if (_projectScript.first != StringId() && _projectScript.first == _targetToScript[target]) {
        _projectScript.second->onUnload();
        // ComponentManager::unregisterCustomComponents();

        _projectScript.first = StringId();
        delete _projectScript.second;
        _projectScript.second = nullptr;
    }

    _targetToScript.erase(target);

    // Release target
    _linker->releaseTarget(target);
}
} // namespace atta::script
