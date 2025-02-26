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

unsigned ComponentRegistry::getSerializedSize(Component* component) {
    std::ostringstream of;
    std::basic_ostream<char>::pos_type posBefore = of.tellp();
    serialize(of, component);
    ASSERT(of.tellp() != -1, "Failed to calculate componente serialized size")
    unsigned size = (int)of.tellp() - posBefore;
    return size;
}

} // namespace atta::component
