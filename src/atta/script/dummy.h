//--------------------------------------------------
// Atta Script Module
// dummy.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_DUMMY_H
#define ATTA_SCRIPT_DUMMY_H
#include <atta/script/interface.h>

/**
 * @brief Dummy controller script
 *
 * It is necessary to register some controller at startup to avoid ControllerRegistry from being optimized away
 */
class DummyController : public atta::script::Controller {
  public:
    ATTA_CPU_GPU void update();
};
ATTA_REGISTER_CONTROLLER(DummyController)

/**
 * @brief Dummy world script
 *
 * It is necessary to register some world at startup to avoid WorldRegistry from being optimized away
 */
class DummyWorld : public atta::script::World {};
ATTA_REGISTER_WORLD(DummyWorld)

#endif // ATTA_SCRIPT_DUMMY_H
