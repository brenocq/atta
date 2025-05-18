//--------------------------------------------------
// Atta UI Module
// viewport.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>
#include <atta/ui/windows/viewport/viewport.h>
#include <imgui.h>

namespace atta::ui {

size_t Viewport::_viewportCount = 0;

Viewport::Viewport() : Viewport(CreateInfo{}) {}

Viewport::Viewport(CreateInfo info) : _renderer(info.renderer), _camera(info.camera) {
    _sid = StringId("AttaViewport[" + std::to_string(_viewportCount++) + "]");
    _name = info.name;

    unsigned i = 0;
    for (auto c : _name)
        _inputText[i++] = c;
    _inputText[i] = '\0';
}

Viewport::~Viewport() {}

void Viewport::render() {
    if (_newRenderer) {
        _renderer.reset();
        _renderer = _newRenderer;
        _newRenderer.reset();
    }
    _renderer->render(_camera);
}

void Viewport::resize(uint32_t width, uint32_t height) {
    _camera->setRatio(width / (float)height);
    _renderer->resize(width, height);
}

void Viewport::renderUI() {
    //---------- Name ----------//
    ImGui::Text("Name");
    ImGui::InputText(("##Name" + _sid.getString()).c_str(), _inputText, sizeof(_inputText) / sizeof(char));
    _name = std::string(_inputText);

    //---------- Renderer ----------//
    ImGui::Separator();
    ImGui::Text("Renderer");
    std::vector<const char*> renderers = {"FastRenderer", "PhongRenderer", "PbrRenderer"};

    unsigned comboValue = 0;
    for (size_t j = 0; j < renderers.size(); j++) {
        if (_renderer->getName() == renderers[j]) {
            comboValue = j;
            break;
        }
    }

    if (ImGui::BeginCombo("Type###RendererType", renderers[comboValue])) {
        for (size_t j = 0; j < renderers.size(); j++) {
            if (ImGui::Selectable(renderers[j], comboValue == j)) {
                switch (j) {
                    case 0:
                        if (comboValue != 0)
                            setRenderer(std::static_pointer_cast<gfx::Renderer>(std::make_shared<gfx::FastRenderer>()));
                        break;
                    case 1:
                        if (comboValue != 1)
                            setRenderer(std::static_pointer_cast<gfx::Renderer>(std::make_shared<gfx::PhongRenderer>()));
                        break;
                    case 2:
                        if (comboValue != 2)
                            setRenderer(std::static_pointer_cast<gfx::Renderer>(std::make_shared<gfx::PbrRenderer>()));
                        break;
                }
            }
            if (comboValue == j)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    //---------- Camera ----------//
    ImGui::Separator();
    ImGui::Text("Camera");

    std::vector<const char*> cameras = {"OrthographicCamera", "PerspectiveCamera"};

    comboValue = 0;
    for (size_t j = 0; j < renderers.size(); j++) {
        if (_camera->getName() == cameras[j]) {
            comboValue = j;
            break;
        }
    }

    if (ImGui::BeginCombo("Type###CameraType", cameras[comboValue])) {
        for (size_t j = 0; j < cameras.size(); j++) {
            if (ImGui::Selectable(cameras[j], comboValue == j)) {
                switch (j) {
                    case 0:
                        if (comboValue != 0)
                            setCamera(std::make_shared<gfx::OrthographicCamera>(gfx::OrthographicCamera::CreateInfo{}));
                        break;
                    case 1:
                        if (comboValue != 1)
                            setCamera(std::make_shared<gfx::PerspectiveCamera>(gfx::PerspectiveCamera::CreateInfo{}));
                        break;
                }
            }
            if (comboValue == j)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    _camera->renderUI();
}

} // namespace atta::ui
