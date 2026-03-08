// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/mesh.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Mesh>::getDescription() {
    static ComponentDescription desc = {
        "Mesh",
        {
            {AttributeType::STRINGID, offsetof(Mesh, sid), "sid", {}, {}, {}, {}},
        },
        1024, // Max instances
    };

    return desc;
}

Mesh::Mesh() { sid = *resource::getResources<resource::Mesh>().begin(); }

void Mesh::set(std::string mesh) { sid = StringId(mesh); }

} // namespace atta::component
