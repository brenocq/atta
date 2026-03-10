// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/environmentLight.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<EnvironmentLight>::getDescription() {
    static ComponentDescription desc = {"Environment Light",
                                        {
                                            {AttributeType::STRINGID, offsetof(EnvironmentLight, sid), "sid", {}, {}, {}, {}},
                                        },
                                        1};

    return desc;
}

EnvironmentLight::EnvironmentLight() {
    if (TypedComponentRegistry<EnvironmentLight>::description->attributeDescriptions[0].options.size())
        sid = StringId(*(TypedComponentRegistry<EnvironmentLight>::description->attributeDescriptions[0].options.begin()));
}

} // namespace atta::component
