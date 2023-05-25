//--------------------------------------------------
// Atta Script Module
// manager.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_MANAGER_H
#define ATTA_SCRIPT_MANAGER_H

#include <atta/event/event.h>
#ifdef ATTA_DYNAMIC_PROJECT
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
    friend std::vector<StringId> getScriptSids();

  private:
    void startUpImpl();
    void shutDownImpl();

    void updateImpl(float dt);

    std::vector<StringId> getScriptSidsImpl() const;

    // Handle events
    void onProjectOpen(event::Event& event);
    void onProjectClose(event::Event& event);

#ifdef ATTA_DYNAMIC_PROJECT
    void onFileChange(event::Event& event);
    void onProjectBeforeDeserialize(event::Event& event);

    void updateAllTargets();
    void updateTarget(StringId target);

    std::shared_ptr<Compiler> _compiler;
    std::shared_ptr<Linker> _linker;
#endif
};

} // namespace atta::script

#endif // ATTA_SCRIPT_MANAGER_H
