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
        virtual void onLoad() = 0;
        virtual void onUnload() = 0;

        //---------- Simulation ----------//
        virtual void onStart() = 0;
        virtual void onStop() = 0;

        virtual void onContinue() = 0;
        virtual void onPause() = 0;

        virtual void onUpdateBefore(float delta) = 0;
        virtual void onUpdateAfter(float delta) = 0;

        //---------- Editor ----------//
        virtual void onUIUpdate() = 0;
        virtual void onUIRender() = 0;
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
