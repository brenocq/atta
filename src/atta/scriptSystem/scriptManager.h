//--------------------------------------------------
// Atta Script System
// scriptManager.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_SCRIPT_MANAGER_H
#define ATTA_SCRIPT_SYSTEM_SCRIPT_MANAGER_H
#include <atta/eventSystem/event.h>
#include <atta/scriptSystem/compilers/compiler.h>
#include <atta/scriptSystem/linkers/linker.h>

namespace atta
{
    class ScriptManager final
    {
    public:
        static ScriptManager& getInstance();
        static void startUp();
        static void shutDown();

        static Script* getScript(StringId target);
        static std::vector<StringId> getScriptSids();

        static ProjectScript* getProjectScript();
        static StringId getProjectScriptSid();

    private:
        void startUpImpl();
        void shutDownImpl();

        Script* getScriptImpl(StringId target) const;
        std::vector<StringId> getScriptSidsImpl() const;

        ProjectScript* getProjectScriptImpl() const;
        StringId getProjectScriptSidImpl() const;

        // Handle events
        void onFileChange(Event& event);
        void onProjectOpen(Event& event);
        void onProjectClose(Event& event);

        void updateAllTargets();
        void updateTarget(StringId target);
        void linkTarget(StringId target);
        void releaseTarget(StringId target);

        std::shared_ptr<Compiler> _compiler;
        std::shared_ptr<Linker> _linker;

        std::unordered_map<StringId, Script*> _scripts;
        std::pair<StringId, ProjectScript*> _projectScript;

    };
}

#endif// ATTA_SCRIPT_SYSTEM_SCRIPT_MANAGER_H
