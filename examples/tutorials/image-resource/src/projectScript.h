//--------------------------------------------------
// Image Resource
// projectScript.h
// Date: 2022-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onLoad() override;
    void onAttaLoop() override;

  private:
    void updateAnimation();
    void updateGameOfLife();
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
