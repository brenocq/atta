//--------------------------------------------------
// Simple Script
// cubeScript.h
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CUBE_SCRIPT_H
#define CUBE_SCRIPT_H
#include <atta/script/interface.h>

class CubeScript : public scr::Script {
  public:
    void update(cmp::Entity entity, float dt) override;
};

ATTA_REGISTER_SCRIPT(CubeScript)

#endif // CUBE_SCRIPT_H
