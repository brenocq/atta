//--------------------------------------------------
// Atta Graphics Module
// camera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/cameras/camera.h>
#include <imgui_internal.h>

namespace atta::graphics {

void Camera::update() {
    _front = normalize(_front);
    _left = cross(_up, _front);
    _up = cross(_front, _left);

    _left.normalize();
    _up.normalize();
}

void Camera::move() {
    switch (_control) {
        case Control::PLANAR:
            movePlanar();
            break;
        case Control::FIRST_PERSON:
            moveFirstPerson();
            break;
        default:
            break;
    }
}

void Camera::movePlanar() {
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) // Move only if middle button is pressed
        return;

    ImGuiIO& io = ImGui::GetIO();

    float x = io.MouseDelta.x * 0.01f;
    float y = io.MouseDelta.y * 0.01f;

    _position += x * _left + -y * _up;
}

void Camera::moveFirstPerson() {
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) // Move only if middle button is pressed
        return;

    static float lastTimeFront = 0;
    static float lastTimeLeft = 0;
    static float lastTimeUp = 0;

    ImGuiIO& io = ImGui::GetIO();

    float x = io.MouseDelta.x * 0.01f;
    float y = io.MouseDelta.y * 0.01f;

    mat4 orientation = mat4(1.0f);
    orientation = rotationFromAxisAngle(-_up, x) * rotationFromAxisAngle(_left, y);

    _front = orientation * _front;
    _up = vec3(0, 0, 1);

    // Move front/back
    if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_S)) {
        int key = 0;

        if (io.KeysData[ImGuiKey_W].DownDuration > 0 && io.KeysData[ImGuiKey_S].DownDuration > 0)
            if (io.KeysData[ImGuiKey_W].DownDuration < io.KeysData[ImGuiKey_S].DownDuration)
                key = ImGuiKey_W;
            else
                key = ImGuiKey_S;
        else if (io.KeysData[ImGuiKey_W].DownDuration > 0)
            key = ImGuiKey_W;
        else if (io.KeysData[ImGuiKey_S].DownDuration > 0)
            key = ImGuiKey_S;

        if (key) {
            if (io.KeysData[key].DownDuration < lastTimeFront)
                lastTimeFront = 0;

            float delta = (io.KeysData[key].DownDuration - lastTimeFront) * (key == ImGuiKey_W ? 1 : -1) * _speed;
            lastTimeFront = io.KeysData[key].DownDuration;
            _position += _front * delta;
        }
    }

    // Move left/right
    if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_D)) {
        int key = 0;
        if (io.KeysData[ImGuiKey_A].DownDuration > 0 && io.KeysData[ImGuiKey_D].DownDuration > 0)
            if (io.KeysData[ImGuiKey_A].DownDuration < io.KeysData[ImGuiKey_D].DownDuration)
                key = ImGuiKey_A;
            else
                key = ImGuiKey_D;
        else if (io.KeysData[ImGuiKey_A].DownDuration > 0)
            key = ImGuiKey_A;
        else if (io.KeysData[ImGuiKey_D].DownDuration > 0)
            key = ImGuiKey_D;

        if (key) {
            if (io.KeysData[key].DownDuration < lastTimeLeft)
                lastTimeLeft = 0;

            float delta = (io.KeysData[key].DownDuration - lastTimeLeft) * (key == ImGuiKey_A ? 1 : -1) * _speed;
            lastTimeLeft = io.KeysData[key].DownDuration;
            _position += _left * delta;
        }
    }

    // Move up/down
    if (ImGui::IsKeyDown(ImGuiKey_E) || ImGui::IsKeyDown(ImGuiKey_Q)) {
        int key = 0;
        if (io.KeysData[ImGuiKey_E].DownDuration > 0 && io.KeysData[ImGuiKey_Q].DownDuration > 0)
            if (io.KeysData[ImGuiKey_E].DownDuration < io.KeysData[ImGuiKey_Q].DownDuration)
                key = ImGuiKey_E;
            else
                key = ImGuiKey_Q;
        else if (io.KeysData[ImGuiKey_E].DownDuration > 0)
            key = ImGuiKey_E;
        else if (io.KeysData[ImGuiKey_Q].DownDuration > 0)
            key = ImGuiKey_Q;

        if (key) {
            if (io.KeysData[key].DownDuration < lastTimeUp)
                lastTimeUp = 0;

            float delta = (io.KeysData[key].DownDuration - lastTimeUp) * (key == ImGuiKey_E ? 1 : -1) * _speed;
            lastTimeUp = io.KeysData[key].DownDuration;
            _position += _up * delta;
        }
    }

    Camera::update();
}

void Camera::renderUI() {
    std::vector<const char*> controls = {"Planar", "First person"};

    unsigned comboValue = 0;
    if (_control == Control::FIRST_PERSON)
        comboValue = 1;

    if (ImGui::BeginCombo("Control###CameraControl", controls[comboValue])) {
        for (size_t j = 0; j < controls.size(); j++) {
            if (ImGui::Selectable(controls[j], comboValue == j)) {
                switch (j) {
                    case 0:
                        if (comboValue != 0)
                            _control = Control::PLANAR;
                        break;
                    case 1:
                        if (comboValue != 1)
                            _control = Control::FIRST_PERSON;
                        break;
                }
            }
            if (comboValue == j)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

} // namespace atta::graphics
