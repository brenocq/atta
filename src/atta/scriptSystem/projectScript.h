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
        virtual void onUIUpdate() {};
        virtual void onUIRender() {};
    }; 
}

#define ATTA_REGISTER_PROJECT_SCRIPT(DerivedProjectScript) \
extern "C" {\
    atta::ProjectScript* createProjectScript()\
    {\
        return static_cast<atta::ProjectScript*>(new DerivedProjectScript());\
    }\
}

#endif// ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H
