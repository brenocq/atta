//--------------------------------------------------
// Atta UI Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/interface.h>
#include <atta/ui/manager.h>

namespace atta::ui {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

} // namespace atta::ui
