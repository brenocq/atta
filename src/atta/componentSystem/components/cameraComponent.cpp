//--------------------------------------------------
// Atta Component System
// cameraComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/cameraComponent.h>

namespace atta
{
    template<>
    ComponentDescription& TypedComponentRegistry<CameraComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Camera",
                {
                    { AttributeType::UINT32,  offsetof(CameraComponent, width), "width" },
                    { AttributeType::UINT32,  offsetof(CameraComponent, height), "height" },
                    { AttributeType::FLOAT32, offsetof(CameraComponent, fov), "fov", 0.0f, 120.0f },
                    { AttributeType::FLOAT32, offsetof(CameraComponent, far), "far", 0.0f, 10000.0f,  0.5f  },
                    { AttributeType::FLOAT32, offsetof(CameraComponent, near), "near", 0.0f, 10000.0f,  0.5f },
                    { AttributeType::FLOAT32, offsetof(CameraComponent, fps), "fps", 0.0f, 120.0f },
                    { AttributeType::UINT32,  offsetof(CameraComponent, cameraType), "cameraType", {}, {}, {}, 
                        { "Fast", "Phong", "Pbr"} },
                    { AttributeType::UINT32,  offsetof(CameraComponent, rendererType), "rendererType", {}, {}, {}, 
                        { "Orthographic", "Perspective" } }
                }
            };

        return desc;
    }
}
