//--------------------------------------------------
// Entity Spawn
// projectScript.h
// Date: 2022-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/component/interface.h>
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onStart() override;
    void onStop() override;

  private:
    cmp::Entity _redCube;
    cmp::Entity _pointLight;
    std::vector<cmp::Entity> _spheres;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
