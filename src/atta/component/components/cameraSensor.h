//--------------------------------------------------
// Atta Component Module
// cameraSensor.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_CAMERA_SENSOR_H
#define ATTA_COMPONENT_COMPONENTS_CAMERA_SENSOR_H

#include <atta/component/components/component.h>
#include <atta/component/interface.h>
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::component {

/// %Component to create simulated camera
/** A camera is mainly defined by its renderer type and projection
 * matrix (orthographic or perspective). Only objects between the
 * far and near plane are rendered. The fps can be
 * used to reduce how many times the camera image is rendered.
 *
 * The camera direction is the same as the entity X axis, you can
 * change the camera pose by changing the Transform.
 */
struct CameraSensor final : public Component {
    enum class CameraType : uint32_t { ORTHOGRAPHIC = 0, PERSPECTIVE };
    enum class RendererType : uint32_t { FAST = 0, PHONG, PBR };

    bool enabled = true;

    uint32_t width = 128;  ///< Width
    uint32_t height = 128; ///< Height

    float fov = 45.0f;   ///< Field of view
    float far = 100.0f;  ///< Far plane
    float near = 0.001f; ///< Near plane
    float fps = 30;      ///< Frames per second

    CameraType cameraType = CameraType::PERSPECTIVE;
    RendererType rendererType = RendererType::PHONG;

    /// Time when last image was captured
    /** If negative, the first image was not captured yet **/
    float captureTime = -1.0f;

    /// Get image
    /** If captureTime is negative, the first image was not captured
     * yet and a nullptr will be returned
     **/
    const uint8_t* getImage();
};
ATTA_REGISTER_COMPONENT(CameraSensor);
template <>
ComponentDescription& TypedRegistry<CameraSensor>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_CAMERA_SENSOR_H
