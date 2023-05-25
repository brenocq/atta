//--------------------------------------------------
// Atta Script Module
// world.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPTS_WORLD_H
#define ATTA_SCRIPT_SCRIPTS_WORLD_H

namespace atta::script {

class World {
  public:
    //---------- Load/Unload ----------//
    virtual void onLoad();
    virtual void onUnload();

    //---------- Simulation ----------//
    virtual void onStart();
    virtual void onStop();

    virtual void onContinue();
    virtual void onPause();

    virtual void onUpdateBefore(float dt);
    virtual void onUpdateAfter(float dt);

    //---------- Editor ----------//
    virtual void onUIRender();

    //---------- While ----------//
    virtual void onAttaLoop();
};

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPTS_WORLD_H
