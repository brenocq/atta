//--------------------------------------------------
// Atta Script Module
// manager.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_MANAGER_H
#define ATTA_SCRIPT_MANAGER_H

#include <atta/script/projectScript.h>
#include <atta/script/script.h>
#ifndef ATTA_STATIC_PROJECT
#include <atta/event/event.h>
#include <atta/script/compilers/compiler.h>
#include <atta/script/linkers/linker.h>
#endif

namespace atta::script {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update(float dt);
    friend Script* getScript(StringId target);
    friend std::vector<StringId> getScriptSids();
    friend ProjectScript* getProjectScript();
    friend StringId getProjectScriptSid();
#ifdef ATTA_STATIC_PROJECT
    friend ProjectScript* registerProjectScript(std::string name, ProjectScript* projectScript);
    friend Script* registerScript(std::string name, Script* script);
#endif

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl(float dt);

    Script* getScriptImpl(StringId target) const;
    std::vector<StringId> getScriptSidsImpl() const;

    ProjectScript* getProjectScriptImpl() const;
    StringId getProjectScriptSidImpl() const;

    // Handle events
    void onProjectOpen(event::Event& event);
#ifndef ATTA_STATIC_PROJECT
    void onFileChange(event::Event& event);
    void onProjectBeforeDeserialize(event::Event& event);
    void onProjectClose(event::Event& event);

    void updateAllTargets();
    void updateTarget(StringId target);
    void linkTarget(StringId target);
    void releaseTarget(StringId target);

    bool _projectDeserialized;
    std::shared_ptr<Compiler> _compiler;
    std::shared_ptr<Linker> _linker;
    std::map<StringId, StringId> _targetToScript; // Convert target name to script name
#endif

    std::unordered_map<StringId, Script*> _scripts;
    std::pair<StringId, ProjectScript*> _projectScript;
};

} // namespace atta::script

#endif // ATTA_SCRIPT_MANAGER_H
