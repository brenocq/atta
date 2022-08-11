//--------------------------------------------------
// Atta IO Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/io/interface.h>

namespace atta::io {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().startUpImpl(); }
void update() { Manager::getInstance().startUpImpl(); }

} // namespace atta::io
