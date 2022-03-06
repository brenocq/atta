//--------------------------------------------------
// Atta Component System
// cameraComponent.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
	/// %Component to create simulated camera
	/** A camera is mainly defined by its renderer type and projection 
	 * matrix (orthographic or perspective). Only objects between the
	 * far and near plane are rendered. The fps can be
	 * used to reduce how many times the camera image is rendered.
	 *
	 * A TransformComponent is necessary for the camera to be rendered. 
	 * The camera direction is the same as the entity Z axis, you can 
	 * change the camera pose by changing the TransformComponent.
	 */
    struct CameraComponent final : public Component
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

        float fov = 45.0f;///< Field of view
        float far = 100.0f;///< Far plane 
        float near = 0.001f;///< Near plane
        float fps = 30;///< Frames per second

        CameraType cameraType = CameraType::PERSPECTIVE;
        RendererType rendererType = RendererType::PHONG;

        const std::vector<uint8_t>& getFrame();
    };
    ATTA_REGISTER_COMPONENT(CameraComponent);
    template<> ComponentDescription& TypedComponentRegistry<CameraComponent>::getDescription();
}
#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_CAMERA_COMPONENT_H
