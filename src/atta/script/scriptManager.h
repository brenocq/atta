//--------------------------------------------------
// Atta Script Module
// scriptManager.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPT_MANAGER_H
#define ATTA_SCRIPT_SCRIPT_MANAGER_H
#include <atta/script/script.h>
#include <atta/script/projectScript.h>
#ifndef ATTA_STATIC_PROJECT
#include <atta/event/event.h>
#include <atta/script/compilers/compiler.h>
#include <atta/script/linkers/linker.h>
#endif

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

#ifdef ATTA_STATIC_PROJECT
        static ProjectScript* registerProjectScript(std::string name, ProjectScript* projectScript);
        static Script* registerScript(std::string name, Script* script);
#endif

    private:
        void startUpImpl();
        void shutDownImpl();

        Script* getScriptImpl(StringId target) const;
        std::vector<StringId> getScriptSidsImpl() const;

        ProjectScript* getProjectScriptImpl() const;
        StringId getProjectScriptSidImpl() const;

#ifndef ATTA_STATIC_PROJECT
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
        std::map<StringId, StringId> _targetToScript;// Convert target name to script name
#endif

        std::unordered_map<StringId, Script*> _scripts;
        std::pair<StringId, ProjectScript*> _projectScript;
    };
}

#endif// ATTA_SCRIPT_SCRIPT_MANAGER_H
