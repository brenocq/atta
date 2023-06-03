//--------------------------------------------------
// Atta Script Module
// system.h
// Date: 2023-06-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPTS_SYSTEM_H
#define ATTA_SCRIPT_SCRIPTS_SYSTEM_H

namespace atta::script {

class System {
  public:
    //---------- Load/Unload ----------//
    virtual void onLoad();
    virtual void onUnload();

    //---------- Editor ----------//
    virtual void onUIRender();

    //---------- Loop ----------//
    virtual void onAttaLoop();
};

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPTS_SYSTEM_H
