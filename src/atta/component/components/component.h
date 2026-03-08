// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
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
