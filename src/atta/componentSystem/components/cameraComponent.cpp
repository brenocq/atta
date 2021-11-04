//--------------------------------------------------
// Atta Component System
// cameraComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/cameraComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<CameraComponent>::typeDescription = 
    {
        Component::Type("Camera"),
        {
            { Component::AttributeType::UINT32,  attributeOffset(&CameraComponent::width), "width" },
            { Component::AttributeType::UINT32,  attributeOffset(&CameraComponent::height), "height" },
            { Component::AttributeType::FLOAT32, attributeOffset(&CameraComponent::fov), "fov", 0.0f, 120.0f },
            { Component::AttributeType::FLOAT32, attributeOffset(&CameraComponent::far), "far", 0.0f, 10000.0f,  0.5f  },
            { Component::AttributeType::FLOAT32, attributeOffset(&CameraComponent::near), "near", 0.0f, 10000.0f,  0.5f },
            { Component::AttributeType::FLOAT32, attributeOffset(&CameraComponent::fps), "fps", 0.0f, 120.0f },
            { Component::AttributeType::UINT32,  attributeOffset(&CameraComponent::cameraType), "cameraType", {}, {}, {}, { "Fast", "Phong", "Pbr"} },
            { Component::AttributeType::UINT32,  attributeOffset(&CameraComponent::rendererType), "rendererType", {}, {}, {}, { "Orthographic", "Perspective" } }
        }
    };
}
