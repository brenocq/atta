//--------------------------------------------------
// Atta Component System
// cameraComponent.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
    struct CameraComponent : public Component
    {
        enum class CameraType : uint32_t
        {
            ORTHOGRAPHIC = 0,
            PERSPECTIVE
        };

        enum class RendererType : uint32_t
        {
            FAST = 0,
            PHONG,
            PBR
        };

        uint32_t width = 128;
        uint32_t height = 128;

        float fov = 45.0f;
        float far = 100.0f;
        float near = 0.001f;
        float fps = 30;

        CameraType cameraType = CameraType::PERSPECTIVE;
        RendererType rendererType = RendererType::PHONG;
    };
    ATTA_REGISTER_COMPONENT(CameraComponent);

    template<>
    inline const ComponentRegistry::Description TypedComponentRegistry<CameraComponent>::description = 
    {
        "Camera",
        {
            { ComponentRegistry::AttributeType::UINT32,  offsetof(CameraComponent, width), "width" },
            { ComponentRegistry::AttributeType::UINT32,  offsetof(CameraComponent, height), "height" },
            { ComponentRegistry::AttributeType::FLOAT32, offsetof(CameraComponent, fov), "fov", 0.0f, 120.0f },
            { ComponentRegistry::AttributeType::FLOAT32, offsetof(CameraComponent, far), "far", 0.0f, 10000.0f,  0.5f  },
            { ComponentRegistry::AttributeType::FLOAT32, offsetof(CameraComponent, near), "near", 0.0f, 10000.0f,  0.5f },
            { ComponentRegistry::AttributeType::FLOAT32, offsetof(CameraComponent, fps), "fps", 0.0f, 120.0f },
            { ComponentRegistry::AttributeType::UINT32,  offsetof(CameraComponent, cameraType), "cameraType", {}, {}, {}, { "Fast", "Phong", "Pbr"} },
            { ComponentRegistry::AttributeType::UINT32,  offsetof(CameraComponent, rendererType), "rendererType", {}, {}, {}, { "Orthographic", "Perspective" } }
        }
    };
}
#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
