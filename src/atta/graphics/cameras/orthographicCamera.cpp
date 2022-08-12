//--------------------------------------------------
// Atta Graphics Module
// orthographicCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>
#include <atta/graphics/cameras/orthographicCamera.h>
#include <imgui.h>

namespace atta::graphics {

OrthographicCamera::OrthographicCamera(CreateInfo info) : Camera("OrthographicCamera"), _far(info.far), _height(info.height) {
    _position = info.position;
    _front = normalize(info.lookAt - info.position);
    _up = normalize(info.up);
    _left = cross(_front, _up);
    _up = cross(_front, _left);

    _left.normalize();
    _up.normalize();
    _control = info.control;

    _ratio = info.ratio;
}

mat4 OrthographicCamera::getView() const { return lookAt(_position, _position + _front, _up); }

mat4 OrthographicCamera::getProj() const { return orthographic(_height, _ratio, _far); }

void OrthographicCamera::serialize(std::ostream& os) {
    file::write(os, std::string("OrthographicCamera"));
    file::write(os, _position);
    file::write(os, _left);
    file::write(os, _up);
    file::write(os, _front);
    file::write(os, _ratio);
    file::write(os, _control);
    file::write(os, _speed);
    file::write(os, _far);
    file::write(os, _height);
}

void OrthographicCamera::deserialize(std::istream& is) {
    file::read(is, _position);
    file::read(is, _left);
    file::read(is, _up);
    file::read(is, _front);
    file::read(is, _ratio);
    file::read(is, _control);
    file::read(is, _speed);
    file::read(is, _far);
    file::read(is, _height);
}

void OrthographicCamera::movePlanar() {
    ImGuiIO& io = ImGui::GetIO();
    if (_height - io.MouseWheel * 0.5 > 0)
        _height -= io.MouseWheel * 0.5;

    // Move only if middle button is pressed
    if (ImGui::IsMouseDown(2)) {
        ImGuiIO& io = ImGui::GetIO();

        float x = io.MouseDelta.x / float(_viewportWidth) * _height * _ratio;
        float y = io.MouseDelta.y / float(_viewportHeight) * _height;

        _position += -x * _left + -y * _up;
    }
}

} // namespace atta::graphics
