//--------------------------------------------------
// Atta Graphics Module
// perspectiveCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>
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

void PerspectiveCamera::serialize(std::ostream& os) {
    file::write(os, std::string("PerspectiveCamera"));
    file::write(os, _position);
    file::write(os, _left);
    file::write(os, _up);
    file::write(os, _front);
    file::write(os, _ratio);
    file::write(os, _control);
    file::write(os, _speed);
    file::write(os, _near);
    file::write(os, _far);
    file::write(os, _fov);
}

void PerspectiveCamera::deserialize(std::istream& is) {
    file::read(is, _position);
    file::read(is, _left);
    file::read(is, _up);
    file::read(is, _front);
    file::read(is, _ratio);
    file::read(is, _control);
    file::read(is, _speed);
    file::read(is, _near);
    file::read(is, _far);
    file::read(is, _fov);
}

} // namespace atta::graphics
