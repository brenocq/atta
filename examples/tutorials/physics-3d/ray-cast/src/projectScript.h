//--------------------------------------------------
// Physics 3D - Ray Cast
// projectScript.h
// Date: 2022-10-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onUpdateBefore(float delta) override;
    void onAttaLoop() override;
    void onUIRender() override;

  private:
    bool _onlyFirst = false;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
