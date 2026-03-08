//--------------------------------------------------
// Simple Script
// lightScript.h
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHT_SCRIPT_H
#define LIGHT_SCRIPT_H
#include <atta/script/interface.h>

class LightScript : public scr::Script {
  public:
    void update(cmp::Entity entity, float dt) override;
};

ATTA_REGISTER_SCRIPT(LightScript)

#endif // LIGHT_SCRIPT_H
