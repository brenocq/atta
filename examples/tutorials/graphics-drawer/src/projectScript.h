//--------------------------------------------------
// Graphics Drawer
// projectScript.h
// Date: 2022-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onLoad() override;

    void onUpdateBefore(float dt) override;
    void onStop() override;

    void onUnload() override;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
