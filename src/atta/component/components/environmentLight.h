//--------------------------------------------------
// Atta Component Module
// environmentLight.h
// Date: 2022-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_H
#define ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_H

#include <atta/component/interface.h>

namespace atta::component {

struct EnvironmentLight final : public Component {
    EnvironmentLight();
    StringId sid; ///< Environment texture relative path
};
ATTA_REGISTER_COMPONENT(EnvironmentLight)
template <>
ComponentDescription& TypedComponentRegistry<EnvironmentLight>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_H
