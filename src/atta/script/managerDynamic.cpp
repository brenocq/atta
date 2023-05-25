//--------------------------------------------------
// Atta Script Module
// managerDynamic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/interface.h>
#include <atta/event/events/fileWatch.h>
#include <atta/event/events/projectBeforeDeserialize.h>
#include <atta/script/compilers/linuxCompiler.h>
#include <atta/script/compilers/nullCompiler.h>
#include <atta/script/linkers/linuxLinker.h>
#include <atta/script/linkers/nullLinker.h>

namespace atta::script {

void Manager::startUpImpl() {
#ifdef ATTA_OS_LINUX
    _compiler = std::static_pointer_cast<Compiler>(std::make_shared<LinuxCompiler>());
    _linker = std::static_pointer_cast<Linker>(std::make_shared<LinuxLinker>());
#else
    _compiler = std::static_pointer_cast<Compiler>(std::make_shared<NullCompiler>());
    _linker = std::static_pointer_cast<Linker>(std::make_shared<NullLinker>());
#endif

    event::subscribe<event::FileWatch>(BIND_EVENT_FUNC(Manager::onFileChange));
    event::subscribe<event::ProjectBeforeDeserialize>(BIND_EVENT_FUNC(Manager::onProjectBeforeDeserialize));
    event::subscribe<event::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));
    event::subscribe<event::ProjectClose>(BIND_EVENT_FUNC(Manager::onProjectClose));
}

void Manager::shutDownImpl() {
    // Link each target in the project
    for (auto target : _compiler->getTargets())
        _linker->linkTarget(target);
}

void Manager::onFileChange(event::Event& event) {
    event::FileWatch& e = reinterpret_cast<event::FileWatch&>(event);

    if (e.file.filename() == "CMakeLists.txt") {
        updateAllTargets();
    } else {
        for (auto target : _compiler->getTargetFiles())
            for (auto file : target.second)
                if (file == e.file)
                    updateTarget(target.first);
    }

    // Publish event
    event::ScriptTarget evt;
    evt.scriptSids = getScriptSidsImpl();
    event::publish(evt);
}

void Manager::onProjectBeforeDeserialize(event::Event& event) {
    // Load scripts and components. It is necessary to do it before deserializing
    // the project to be able to deserialize custom components
    updateAllTargets();

    // Publish event
    event::ScriptTarget evt;
    evt.scriptSids = getScriptSidsImpl();
    event::publish(evt);
}

void Manager::onProjectOpen(event::Event& event) {
    // Call onLoad on all world scripts
    WorldRegistry::onLoad();
}

void Manager::onProjectClose(event::Event& event) {
    // Call onUnload on all world scripts
    WorldRegistry::onUnload();

    // Release all targets
    for (auto target : _compiler->getTargets())
        _linker->releaseTarget(target);
}

void Manager::updateAllTargets() {
    // Release all targets
    for (auto target : _compiler->getTargets())
        _linker->releaseTarget(target);

    // Recompile all targets
    _compiler->compileAll();

    // Link each target in the project
    for (auto target : _compiler->getTargets())
        _linker->linkTarget(target);
}

void Manager::updateTarget(StringId target) {
    // Delete all scripts related to this target
    _linker->releaseTarget(target);

    // Compile and link specific target
    _compiler->compileTarget(target);

    // Link target
    _linker->linkTarget(target);
}

} // namespace atta::script
