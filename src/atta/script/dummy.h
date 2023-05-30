//--------------------------------------------------
// Atta Script Module
// dummy.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_DUMMY_H
#define ATTA_SCRIPT_DUMMY_H
#include <atta/script/scripts/controller.h>
#include <atta/script/scripts/world.h>

/**
 * @brief Dummy controller script
 *
 * It is necessary to register some controller at startup to avoid ControllerRegistry from being optimized away
 */
class DummyController : public atta::script::Controller {
  public:
    ATTA_CPU_GPU void update();
};

/**
 * @brief Dummy world script
 *
 * It is necessary to register some world at startup to avoid WorldRegistry from being optimized away
 */
class DummyWorld : public atta::script::World {};

#endif // ATTA_SCRIPT_DUMMY_H
