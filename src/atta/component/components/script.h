// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/interface.h>

namespace atta::component {

/// Script to be executed by the entity
/** Each entity can only have one script attached to it.
 * scripts are defined in the project folder and the script
 * name is the cmake target name.
 */
struct Script final : public Component {
    Script();
    /** The script name is the same as the cmake target name */
    StringId sid; ///< Script name
    void set(std::string script);
};
ATTA_REGISTER_COMPONENT(Script)
template <>
ComponentDescription& TypedComponentRegistry<Script>::getDescription();

} // namespace atta::component
