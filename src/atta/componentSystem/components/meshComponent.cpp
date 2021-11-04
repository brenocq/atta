//--------------------------------------------------
// Atta Component System
// meshComponent.cpp
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/meshComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<MeshComponent>::typeDescription = 
    {
        Component::Type("Mesh"),
        {
            { 
                Component::AttributeType::STRINGID, 
                attributeOffset(&MeshComponent::sid), 
                "sid", 
                {},
                {},
                {},
                { "test" }
            },
        }
    };
}
