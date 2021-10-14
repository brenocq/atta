//--------------------------------------------------
// Atta Component System
// cameraComponent.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
    struct CameraComponent
    {
        enum class CameraType : uint8_t
        {
            ORTHOGRAPHIC = 0,
            PERSPECTIVE
        };

        enum class RendererType : uint8_t
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
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
