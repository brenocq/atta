//--------------------------------------------------
// Atta Script System
// projectScript.h
// Date: 2021-09-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H
#define ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H

namespace atta
{
    class ProjectScript               
    {
    public:
        ProjectScript() = default;    
        virtual ~ProjectScript() {};  

        //---------- Load/Unload ----------//
        virtual void onLoad() {};
        virtual void onUnload() {};

        //---------- Simulation ----------//
        virtual void onStart() {};
        virtual void onStop() {};

        virtual void onContinue() {};
        virtual void onPause() {};

        virtual void onUpdateBefore(float delta) {};
        virtual void onUpdateAfter(float delta) {};

        //---------- Editor ----------//
        virtual void onUIRender() {};

        //---------- While ----------//
        virtual void onAttaLoop() {};
    }; 

#ifdef ATTA_STATIC_PROJECT
    template<typename T> class ProjectScriptRegistration { static ProjectScript* reg; };
#define ATTA_REGISTER_PROJECT_SCRIPT(TYPE) \
    template<> inline ::atta::ProjectScript* ::atta::ProjectScriptRegistration<TYPE>::reg = ::atta::ScriptManager::registerProjectScript(#TYPE, new TYPE());
#else
#define ATTA_REGISTER_PROJECT_SCRIPT(TYPE) \
    extern "C" {\
        std::pair<const char*, atta::ProjectScript*> createProjectScript()\
        {\
            return { #TYPE, static_cast<atta::ProjectScript*>(new TYPE()) };\
        }\
    }
#endif

}

#endif// ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H
