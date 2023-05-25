//--------------------------------------------------
// Atta Script Module
// controller.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/scripts/controller.h>

namespace atta::script {

Controller::Controller() : _entity(-1), _dt(0.0f) {}

Controller::Controller(cmp::Entity entity, float dt) : _entity(entity), _dt(dt) {}

} // namespace atta::script
