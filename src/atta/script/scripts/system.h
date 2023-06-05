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
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;

    //---------- Editor ----------//
    virtual void onUIRender() = 0;

    //---------- Loop ----------//
    virtual void onAttaLoop() = 0;
};

} // namespace atta::script

#endif // ATTA_SCRIPT_SCRIPTS_SYSTEM_H
