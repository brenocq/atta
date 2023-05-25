//--------------------------------------------------
// Atta Script Module
// controller.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPTS_CONTROLLER_H
#define ATTA_SCRIPT_SCRIPTS_CONTROLLER_H

#include <atta/component/interface.h>

namespace atta::script {

class Controller {
  public:
    Controller();
    Controller(cmp::Entity entity, float dt);

    ATTA_CPU_GPU virtual void update() = 0;

    template <typename T>
    ATTA_CPU_GPU T* get() const;

  protected:
    cmp::Entity _entity;
    float _dt;
};

} // namespace atta::script

#include <atta/script/scripts/controller.inl>

#endif // ATTA_SCRIPT_SCRIPTS_CONTROLLER_H
