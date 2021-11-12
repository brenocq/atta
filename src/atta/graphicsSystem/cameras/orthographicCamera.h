//--------------------------------------------------
// Atta Graphics System
// orthographicCamera.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_CAMERAS_ORTHOGRAPHIC_CAMERA_H
#define ATTA_GRAPHICS_SYSTEM_CAMERAS_ORTHOGRAPHIC_CAMERA_H
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
    class OrthographicCamera : public Camera
    {
    public:
        struct CreateInfo
        {
            vec3 position = {0.0f, 0.0f, 3.0f};
            vec3 lookAt = {0.0f, 0.0f, 0.0f};
            vec3 up = {0.0f, 1.0f, 0.0f};
            Control control = Control::PLANAR;
            float far = 1000.0f;
            float height = 5.0f;
            float ratio = 1.0f;
        };

        OrthographicCamera(CreateInfo info);
        void serialize(std::ostream& os) override;
        void deserialize(std::istream& is) override;

        mat4 getView() const override;
        mat4 getProj() const override;

    private:
        float _far;
        float _height;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_CAMERAS_ORTHOGRAPHIC_CAMERA_H
