//--------------------------------------------------
// Atta Processor Module
// serial.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/processor/processors/serial.h>

namespace atta::processor {

Serial::Serial() : Processor(Type::SERIAL) {}

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

void Serial::start() {}
void Serial::pause() {}
void Serial::resume() {}
void Serial::stop() {}

} // namespace atta::processor
