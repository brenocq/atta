//--------------------------------------------------
// Atta Graphics Module
// perspectiveCamera.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CAMERAS_PERSPECTIVE_CAMERA_H
#define ATTA_GRAPHICS_CAMERAS_PERSPECTIVE_CAMERA_H
#include <atta/graphics/cameras/camera.h>

namespace atta
{
    class PerspectiveCamera : public Camera
    {
    public:
        struct CreateInfo
        {
            vec3 position = {2.0f, 2.0f, 2.0f};
            vec3 lookAt = {0.0f, 0.0f, 0.0f};
            vec3 up = {0.0f, 0.0f, 1.0f};
            Control control = Control::FIRST_PERSON;
            float near = 0.01f;
            float far = 100.0f;
            float fov = 45.0f;
            float ratio = 1.0f;
        };

        PerspectiveCamera(CreateInfo info);
        void serialize(std::ostream& os) override;
        void deserialize(std::istream& is) override;

        mat4 getView() const override;
        mat4 getProj() const override;
        void setFov(float fov) { _fov = fov; }

    private:
        float _near;
        float _far;
        float _fov;
    };
}

#endif// ATTA_GRAPHICS_CAMERAS_PERSPECTIVE_CAMERA_H
