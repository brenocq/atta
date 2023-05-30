//--------------------------------------------------
// Atta Script Module
// controller.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPTS_CONTROLLER_H
#define ATTA_SCRIPT_SCRIPTS_CONTROLLER_H

#include <atta/component/entity.h>

namespace atta::script {

/**
 * @brief Controller script
 *
 * When deriving this class, the update function must be implemented:
 *   - ATTA_CPU_GPU void update();
 */
class Controller {
  public:
    template <typename T>
    ATTA_CPU_GPU T* get() const;

    cmp::Entity entity;
    float dt;
};

} // namespace atta::script

#include <atta/script/scripts/controller.inl>

#endif // ATTA_SCRIPT_SCRIPTS_CONTROLLER_H
