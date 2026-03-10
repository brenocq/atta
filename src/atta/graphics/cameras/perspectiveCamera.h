// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/graphics/cameras/camera.h>

namespace atta::graphics {

class PerspectiveCamera : public Camera {
  public:
    struct CreateInfo {
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

    mat4 getView() const override;
    mat4 getProj() const override;
    void setFov(float fov) { _fov = fov; }

  private:
    float _near;
    float _far;
    float _fov;
};

} // namespace atta::graphics
