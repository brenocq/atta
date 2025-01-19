//--------------------------------------------------
// Atta UI Module
// viewport.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>
#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>
#include <atta/ui/windows/viewport/viewport.h>
#include <imgui.h>

namespace atta::ui {

Viewport::Viewport() : Viewport(CreateInfo{}) {}

Viewport::Viewport(CreateInfo info) : _sid(info.sid), _renderer(info.renderer), _camera(info.camera) {
    if (info.name.size() == 0)
        _name = _sid.getString();
    else
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

void Viewport::serialize(std::ostream& os) {
    //----- Write viewport name -----//
    file::write(os, _sid);
    file::write(os, _name);

    //----- Write renderer -----//
    if (_renderer)
        _renderer->serialize(os);
    else
        file::write(os, std::string(""));

    //----- Write camera -----//
    if (_camera)
        _camera->serialize(os);
    else
        file::write(os, std::string(""));
}

void Viewport::deserialize(std::istream& is) {
    //----- Read viewport name -----//
    // Read string
    file::read(is, _sid);
    file::read(is, _name);
    unsigned i = 0;
    for (auto c : _name)
        _inputText[i++] = c;
    _inputText[i] = '\0';

    //----- Read renderer -----//
    std::string rendererName;
    file::read(is, rendererName);
    if (rendererName == "FastRenderer") {
        _renderer = std::make_shared<gfx::FastRenderer>();
        _renderer->deserialize(is);
    } else if (rendererName == "PhongRenderer") {
        _renderer = std::make_shared<gfx::PhongRenderer>();
        _renderer->deserialize(is);
    } else if (rendererName == "PbrRenderer") {
        _renderer = std::make_shared<gfx::PbrRenderer>();
        _renderer->deserialize(is);
    } else if (rendererName.size() == 0) {
        LOG_WARN("graphics::Viewport", "No renderer deserialized, using default renderer");
        _renderer = std::make_shared<gfx::PhongRenderer>();
    } else {
        LOG_WARN("graphics::Viewport", "Unknown renderer [w]$0[] when deserializing", rendererName);
        _renderer = std::make_shared<gfx::PhongRenderer>();
        _renderer->deserialize(is);
    }

    //----- Read camera -----//
    std::string cameraName;
    file::read(is, cameraName);
    if (cameraName == "OrthographicCamera") {
        _camera = std::make_shared<gfx::OrthographicCamera>(gfx::OrthographicCamera::CreateInfo{});
        _camera->deserialize(is);
    } else if (cameraName == "PerspectiveCamera") {
        _camera = std::make_shared<gfx::PerspectiveCamera>(gfx::PerspectiveCamera::CreateInfo{});
        _camera->deserialize(is);
    } else if (cameraName.size() == 0) {
        LOG_WARN("graphics::Viewport", "No camera deserialized, using default camera");
        _camera = std::make_shared<gfx::PerspectiveCamera>(gfx::PerspectiveCamera::CreateInfo{});
    } else {
        LOG_WARN("graphics::Viewport", "Unknown camera [w]$0[] when deserializing. This may break the serialization", cameraName);
        _camera = std::make_shared<gfx::PerspectiveCamera>(gfx::PerspectiveCamera::CreateInfo{});
    }
}

void Viewport::renderUI() {
    //---------- Name ----------//
    ImGui::Text("Name");
    ImGui::InputText(("##ViewportName" + _sid.getString()).c_str(), _inputText, sizeof(_inputText) / sizeof(char));
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
