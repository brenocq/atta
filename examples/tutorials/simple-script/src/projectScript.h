//--------------------------------------------------
// Simple Script
// projectScript.h
// Date: 2021-11-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/script/interface.h>

class Project : public scr::ProjectScript {
  public:
    //---------- Load/Unload ----------//
    void onLoad() override;
    void onUnload() override;

    //---------- Simulation ----------//
    void onStart() override;
    void onStop() override;

    void onContinue() override;
    void onPause() override;

    void onUpdateBefore(float delta) override;
    void onUpdateAfter(float delta) override;

    //---------- Editor ----------//
    void onUIRender() override;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
