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

namespace atta
{
    Viewport::Viewport(CreateInfo info):
        _sid(info.sid), _renderer(info.renderer), _camera(info.camera)
    {
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
}
