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
    ATTA_CPU_GPU virtual void update() = 0;

    template <typename T>
    ATTA_CPU_GPU T* get() const;

    cmp::Entity entity;
    float dt;
};

} // namespace atta::script

#include <atta/script/scripts/controller.inl>

#endif // ATTA_SCRIPT_SCRIPTS_CONTROLLER_H
