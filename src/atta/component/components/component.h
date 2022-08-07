//--------------------------------------------------
// Atta Component Module
// component.h
// Date: 2021-11-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_H
#define ATTA_COMPONENT_COMPONENTS_H

namespace atta::component {

/// Base class for all components
struct Component {
  public:
    ~Component() = default;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_H
