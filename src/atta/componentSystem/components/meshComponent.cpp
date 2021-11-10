//--------------------------------------------------
// Atta Component System
// meshComponent.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/meshComponent.h>

namespace atta
{
    MeshComponent::MeshComponent()
    {
        if(TypedComponentRegistry<MeshComponent>::description.attributeDescriptions[0].options.size())
            sid = std::any_cast<StringId>(*(TypedComponentRegistry<MeshComponent>::description.attributeDescriptions[0].options.begin()));
    }
}
