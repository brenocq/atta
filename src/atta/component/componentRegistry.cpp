//--------------------------------------------------
// Atta Component Module
// componentRegistry.cpp
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/interface.h>
#include <imgui.h>

namespace atta::component {

void ComponentRegistry::registerToManager() { component::registerComponent(this); }

} // namespace atta::component
