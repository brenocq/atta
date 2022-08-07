//--------------------------------------------------
// Atta Component Module
// nameComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_NAME_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_NAME_COMPONENT_H
#include <atta/component/componentManager.h>

namespace atta::component {
/// %Component to give an entity a name
struct Name final : public Component {
    char name[32];
};
ATTA_REGISTER_COMPONENT(NameComponent)
template <>
ComponentDescription& TypedComponentRegistry<NameComponent>::getDescription();
} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_NAME_COMPONENT_H
