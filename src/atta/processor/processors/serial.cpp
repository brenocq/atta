//--------------------------------------------------
// Atta Processor Module
// serial.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/serial.h>

namespace atta::processor {

Serial::Serial() : Processor(Type::SERIAL) { LOG_DEBUG("Serial", "Porcessor -> $0", (int)_state); }

Serial::~Serial() {}

// template <typename Script>
// void Serial::run(cmp::Entity entity, float dt, uint32_t num) {
//     cmp::EntityId start = entity.getId();
//     cmp::EntityId end = entity.getId() + num;
//     for (cmp::EntityId i = start; i < end; i++) {
//         Script script;
//         script.entity = cmp::Entity(i);
//         script.dt = dt;
//         script.update();
//     }
// }

void Serial::start() { _state = State::RUNNING; }
void Serial::pause() { _state = State::PAUSED; }
void Serial::resume() { _state = State::RUNNING; }
void Serial::stop() { _state = State::IDLE; }

} // namespace atta::processor
