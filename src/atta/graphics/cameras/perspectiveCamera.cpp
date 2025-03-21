//--------------------------------------------------
// Atta Graphics Module
// perspectiveCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/cameras/perspectiveCamera.h>

namespace atta::graphics {

PerspectiveCamera::PerspectiveCamera(CreateInfo info) : Camera("PerspectiveCamera"), _near(info.near), _far(info.far), _fov(info.fov) {
    _position = info.position;
    _front = normalize(info.lookAt - info.position);
    _up = normalize(info.up);
    _left = cross(_up, _front);
    _up = cross(_front, _left);

    _left.normalize();
    _up.normalize();
    _control = info.control;
    _speed = 10.0f;

    _ratio = info.ratio;
}

mat4 PerspectiveCamera::getView() const { return lookAt(_position, _position + _front, _up); }

mat4 PerspectiveCamera::getProj() const {
    mat4 proj = perspective(_fov, _ratio, _near, _far);
    proj.mat[1][1] *= -1;
    return proj;
}

} // namespace atta::graphics
