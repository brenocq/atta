//--------------------------------------------------
// Atta Event Module
// interface.cpp
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/interface.h>

namespace atta::event {

void publish(Event& event) { Manager::getInstance().publishImpl(event); }
void clear() { Manager::getInstance().clearImpl(); }

} // namespace atta::event
