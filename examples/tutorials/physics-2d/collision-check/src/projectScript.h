//--------------------------------------------------
// Physics 2D - Collision Check
// projectScript.h
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onStop() override;

    void onUpdateBefore(float delta) override;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
