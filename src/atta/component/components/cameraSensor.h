// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
ComponentDescription& TypedComponentRegistry<CameraSensor>::getDescription();

} // namespace atta::component
