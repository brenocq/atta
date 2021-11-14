//--------------------------------------------------
// Atta Graphics System
// viewport.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/viewport.h>
#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>
#include <atta/fileSystem/serializer/serializer.h>
#include <imgui.h>

namespace atta
{
    Viewport::Viewport(CreateInfo info):
        _sid(info.sid), _renderer(info.renderer), _camera(info.camera)
    {
        if(info.name.size() == 0)
            _name = _sid.getString();
        else
            _name = info.name;

        _inputText.resize(50);
        unsigned i = 0;
        for(auto c : _name)
            _inputText[i++] = c;
    }

    Viewport::~Viewport()
    {

    }

    void Viewport::render()
    {
        if(_newRenderer)
        {
            _renderer.reset();
            _renderer = _newRenderer;
            _newRenderer.reset();
        }
        _renderer->render(_camera);
    }

    void Viewport::resize(uint32_t width, uint32_t height)
    {
        _camera->setRatio(width/(float)height);
        _renderer->resize(width, height);
    }

    void Viewport::serialize(std::ostream& os)
    {
        //----- Write viewport name -----//
        write(os, _sid);
        write(os, _name);

        //----- Write renderer -----//
        if(_renderer)
            _renderer->serialize(os);
        else
            write(os, std::string(""));

        //----- Write camera -----//
        if(_camera)
            _camera->serialize(os);
        else
            write(os, std::string(""));
    }
    
    void Viewport::deserialize(std::istream& is)
    {
        //----- Read viewport name -----//
        // Read string
        read(is, _sid);
        read(is, _name);
        _inputText.resize(50);
        unsigned i = 0;
        for(auto c : _name)
            _inputText[i++] = c;

        //----- Read renderer -----//
        std::string rendererName;
        read(is, rendererName);
        if(rendererName == "FastRenderer")
        {
            _renderer = std::make_shared<FastRenderer>();
            _renderer->deserialize(is);
        }
        else if(rendererName == "PhongRenderer")
        {
            _renderer = std::make_shared<PhongRenderer>();
            _renderer->deserialize(is);
        }
        else if(rendererName == "PbrRenderer")
        {
            _renderer = std::make_shared<PbrRenderer>();
            _renderer->deserialize(is);
        }
        else if(rendererName.size() == 0)
        {
            LOG_WARN("Viewport", "No renderer deserialized, using default renderer");
            _renderer = std::make_shared<PhongRenderer>();
        }
        else
        {
            LOG_WARN("Viewport", "Unknown renderer [w]$0[] when deserializing", rendererName);
            _renderer = std::make_shared<PhongRenderer>();
            _renderer->deserialize(is);
        }

        //----- Read camera -----//
        std::string cameraName;
        read(is, cameraName);
        if(cameraName == "OrthographicCamera")
        {
            _camera = std::make_shared<OrthographicCamera>(OrthographicCamera::CreateInfo{});
            _camera->deserialize(is);
        }
        else if(cameraName == "PerspectiveCamera")
        {
            _camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{});
            _camera->deserialize(is);
        }
        else if(cameraName.size() == 0)
        {
            LOG_WARN("Viewport", "No camera deserialized, using default camera");
            _camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{});
        }
        else
        {
            LOG_WARN("Viewport", "Unknown camera [w]$0[] when deserializing. This may break the serialization", cameraName);
            _camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{});
        }
        mat4 m = _camera->getProj();
    }

    void Viewport::renderUI()
    {
        //---------- Name ----------//
        ImGui::Text("Name");
        ImGui::InputText(("##ViewportName"+_sid.getString()).c_str(), _inputText.data(), _inputText.size());
        _name = std::string(_inputText.begin(), _inputText.end());

        //---------- Renderer ----------//
        ImGui::Separator();
        ImGui::Text("Renderer");
        std::vector<const char*> renderers = { "FastRenderer", "PhongRenderer", "PbrRenderer" };

        unsigned comboValue = 0;
        for(size_t j = 0; j < renderers.size(); j++)
        {
            if(_renderer->getName() == renderers[j])
            {
                comboValue = j;
                break;
            }
        }

        if(ImGui::BeginCombo("Type###RendererType", renderers[comboValue]))
        {
            for(size_t j = 0; j < renderers.size(); j++)
            {
                if(ImGui::Selectable(renderers[j], comboValue == j))
                {
                    switch(j)
                    {
                    case 0:
                        if(comboValue != 0)
                            setRenderer(std::static_pointer_cast<Renderer>(std::make_shared<FastRenderer>()));
                    break;
                    case 1:
                        if(comboValue != 1)
                            setRenderer(std::static_pointer_cast<Renderer>(std::make_shared<PhongRenderer>()));
                    break;
                    case 2:
                        if(comboValue != 2)
                            setRenderer(std::static_pointer_cast<Renderer>(std::make_shared<PbrRenderer>()));
                    break;
                    }
                }
                if(comboValue == j)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        //---------- Camera ----------//
        ImGui::Separator();
        ImGui::Text("Camera");

        std::vector<const char*> cameras = { "OrthographicCamera", "PerspectiveCamera" };

        comboValue = 0;
        for(size_t j = 0; j < renderers.size(); j++)
        {
            if(_camera->getName() == cameras[j])
            {
                comboValue = j;
                break;
            }
        }

        if(ImGui::BeginCombo("Type###CameraType", cameras[comboValue]))
        {
            for(size_t j = 0; j < cameras.size(); j++)
            {
                if(ImGui::Selectable(cameras[j], comboValue == j))
                {
                    switch(j)
                    {
                        case 0:
                            if(comboValue != 0)
                                setCamera(std::make_shared<OrthographicCamera>(OrthographicCamera::CreateInfo{}));
                        break;
                        case 1:
                            if(comboValue != 1)
                                setCamera(std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{}));
                        break;
                    }
                }
                if(comboValue == j)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        _camera->renderUI();
    }
}
