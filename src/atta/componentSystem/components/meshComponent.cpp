//--------------------------------------------------
// Atta Component System
// meshComponent.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/meshComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<MeshComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Mesh",
                {
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MeshComponent, sid), "sid", {}, {}, {}, {} },
                }
            };

        return desc;
    }

    MeshComponent::MeshComponent()
    {
        if(TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options.size())
            sid = std::any_cast<StringId>(*(TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options.begin()));
    }

}
