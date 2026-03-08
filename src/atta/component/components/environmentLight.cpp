// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/environmentLight.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>

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
    // Set first .hdr image as default image for environment lights
    for (StringId resourceSid : resource::getResources<resource::Image>()) {
        if (resourceSid.getString().find(".hdr") != std::string::npos) {
            sid = resourceSid;
            break;
        }
    }
}

} // namespace atta::component
