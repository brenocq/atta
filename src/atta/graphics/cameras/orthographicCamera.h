//--------------------------------------------------
// Atta Graphics Module
// orthographicCamera.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CAMERAS_ORTHOGRAPHIC_CAMERA_H
#define ATTA_GRAPHICS_CAMERAS_ORTHOGRAPHIC_CAMERA_H

#include <atta/graphics/cameras/camera.h>

namespace atta::graphics {

class OrthographicCamera : public Camera {
  public:
    struct CreateInfo {
        vec3 position = {0.0f, 0.0f, 10.0f};
        vec3 lookAt = {0.0f, 0.0f, 0.0f};
        vec3 up = {0.0f, 1.0f, 0.0f};
        Control control = Control::PLANAR;
        float far = 1000.0f;
        float height = 5.0f;
        float ratio = 1.0f;
    };

    OrthographicCamera(CreateInfo info);

    mat4 getView() const override;
    mat4 getProj() const override;
    float getHeight() const { return _height; }

  private:
    virtual void movePlanar() override;

    float _far;
    float _height;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_CAMERAS_ORTHOGRAPHIC_CAMERA_H
