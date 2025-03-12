//--------------------------------------------------
// Atta Graphics Module
// orthographicCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
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
