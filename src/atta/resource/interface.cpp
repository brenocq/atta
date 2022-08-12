//--------------------------------------------------
// Atta Resource Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resource/interface.h>

namespace atta::resource {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

} // namespace atta::resource
