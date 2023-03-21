//--------------------------------------------------
// Atta Sensor Module
// managerCamera.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::sensor {

void Manager::registerCameras() {
    for (cmp::Entity entity : cmp::getEntitiesView()) {
        cmp::CameraSensor* camera = cmp::getComponent<cmp::CameraSensor>(entity);
        if (camera) {
            // Check if camera was not registered yet
            bool found = false;
            for (auto ci : _cameras)
                if (ci.entity == entity) {
                    found = true;
                    break;
                }
            // Register new camera
            if (!found)
                registerCamera(entity, camera);
        }
    }
}

void Manager::registerCamera(cmp::Entity entity, cmp::CameraSensor* camera) {
    // Do not register prototype cameras
    if (cmp::Entity(entity).isPrototype())
        return;

    CameraInfo cameraInfo{};
    cameraInfo.entity = entity;
    cameraInfo.component = camera;
    cameraInfo.showWindow = false;
    cameraInfo.initialized = false;
    _cameras.push_back(cameraInfo);
}

void Manager::unregisterCameras() { _cameras.clear(); }

void Manager::unregisterCamera(cmp::Entity entity) {
    bool found = false;
    for (int i = _cameras.size() - 1; i >= 0; i--) {
        if (_cameras[i].entity == entity) {
            _cameras.erase(_cameras.begin() + i);
            found = true;
            break;
        }
    }

    if (!found)
        LOG_WARN("sensor::Manager", "Could not unregister camera from entity [w]$0[], camera was not registered before", entity);
}

void Manager::updateCameras(float dt) {
    PROFILE();

    std::vector<CameraInfo*> toRead; // Save which images should read later
    // Send images to be rendered
    {
        PROFILE_NAME("render");
        for (CameraInfo& cameraInfo : _cameras) {
            // Always update camera model (used to render UI sensor drawer)
            updateCameraModel(cameraInfo);

            // Check if it is enabled
            if (!cameraInfo.component->enabled)
                continue;

            // Render if necessary
            float change = Config::getTime() - cameraInfo.component->captureTime;
            float interval = 1.0f / cameraInfo.component->fps;
            if (change >= interval) {
                // Render
                cameraInfo.renderer->render(cameraInfo.camera);
                cameraInfo.component->captureTime = Config::getTime();

                // Start transfer from GPU to CPU
                toRead.push_back(&cameraInfo);
            }
        }
    }

    // Read images transferred from GPU to CPU
    {
        PROFILE_NAME("transfer");
        for (CameraInfo* cameraInfo : toRead)
            cameraInfo->renderer->getFramebuffer()->readImageRequest(0);

        for (CameraInfo* cameraInfo : toRead)
            cameraInfo->data = cameraInfo->renderer->getFramebuffer()->readImage(0);
    }
}

void Manager::initializeCamera(CameraInfo& cameraInfo) {
    // Need to make sure to initialize after the graphics module has started and it is not in the middle of UI rendering
    cmp::CameraSensor* camera = cameraInfo.component;

    // Start with random last time (used to distribute camera rendering across time)
    camera->captureTime = -(rand() / float(RAND_MAX)) / camera->fps;

    if (!cameraInfo.initialized) {
        // If never initialized before, create gfx::Camera and gfx::Renderer
        cameraInfo.initialized = true;

        // Create renderer
        switch (camera->rendererType) {
            case cmp::CameraSensor::RendererType::FAST:
                cameraInfo.renderer = std::make_shared<gfx::FastRenderer>();
                break;
            case cmp::CameraSensor::RendererType::PHONG:
                cameraInfo.renderer = std::make_shared<gfx::PhongRenderer>();
                break;
            case cmp::CameraSensor::RendererType::PBR:
                cameraInfo.renderer = std::make_shared<gfx::PbrRenderer>();
                break;
            default:
                LOG_WARN("sensor::Manager", "Invalid camera renderer type $0 for entity $1", (int)camera->rendererType, cameraInfo.entity);
        }
        cameraInfo.renderer->setRenderDrawer(false);
        cameraInfo.renderer->setRenderSelected(false);
        cameraInfo.renderer->resize(camera->width, camera->height);

        // Create camera (view/projection matrices)
        switch (camera->cameraType) {
            case cmp::CameraSensor::CameraType::ORTHOGRAPHIC: {
                gfx::OrthographicCamera::CreateInfo info{};
                info.height = camera->fov;
                info.far = camera->far;
                info.ratio = camera->width / (float)camera->height;

                cameraInfo.camera = std::static_pointer_cast<gfx::Camera>(std::make_shared<gfx::OrthographicCamera>(info));
                break;
            }
            case cmp::CameraSensor::CameraType::PERSPECTIVE: {
                gfx::PerspectiveCamera::CreateInfo info{};
                info.fov = camera->fov;
                info.far = camera->far;
                info.near = camera->near;
                info.ratio = camera->width / (float)camera->height;
                cameraInfo.camera = std::static_pointer_cast<gfx::Camera>(std::make_shared<gfx::PerspectiveCamera>(info));
                break;
            }
            default:
                LOG_WARN("sensor::Manager", "Invalid camera projection type $0 for entity $1", (int)camera->cameraType, cameraInfo.entity);
        }
    }
}

void Manager::updateCameraModel(CameraInfo& cameraInfo) {
    //----- Update camera pose and parameters -----//
    cmp::Entity entity = cameraInfo.entity;
    cmp::Transform* transform = entity.get<cmp::Transform>();
    if (transform) {
        // Calculate position
        vec3 position = transform->getWorldTransform(entity).position;

        // Calculate orientation
        quat ori = transform->orientation;
        cmp::Entity parent = entity.getParent();
        while (parent.exists()) {
            auto t = parent.get<cmp::Transform>();
            if (t) {
                ori *= t->orientation;
                ori.normalize();
            }
            parent = parent.getParent();
        }

        // Update camera pose
        vec3 front = vec3(1, 0, 0);
        vec3 up = vec3(0, 0, 1);
        ori.rotateVector(front);
        ori.rotateVector(up);
        front.normalize();
        up.normalize();
        cameraInfo.camera->setPosition(position);
        cameraInfo.camera->setFront(front);
        cameraInfo.camera->setUp(up);
        cameraInfo.camera->update();

        // Update camera fov
        if (cameraInfo.component->cameraType == cmp::CameraSensor::CameraType::PERSPECTIVE) {
            std::shared_ptr<gfx::PerspectiveCamera> persCam = std::static_pointer_cast<gfx::PerspectiveCamera>(cameraInfo.camera);
            persCam->setFov(radians(cameraInfo.component->fov));
        }

        // Update renderer
        cmp::CameraSensor::RendererType currRendererType;
        if (cameraInfo.renderer->getName() == "FastRenderer")
            currRendererType = cmp::CameraSensor::RendererType::FAST;
        else if (cameraInfo.renderer->getName() == "PhongRenderer")
            currRendererType = cmp::CameraSensor::RendererType::PHONG;
        else if (cameraInfo.renderer->getName() == "PbrRenderer")
            currRendererType = cmp::CameraSensor::RendererType::PBR;

        if (cameraInfo.component->rendererType != currRendererType) {
            switch (cameraInfo.component->rendererType) {
                case cmp::CameraSensor::RendererType::FAST:
                    cameraInfo.renderer = std::make_shared<gfx::FastRenderer>();
                    break;
                case cmp::CameraSensor::RendererType::PHONG:
                    cameraInfo.renderer = std::make_shared<gfx::PhongRenderer>();
                    break;
                case cmp::CameraSensor::RendererType::PBR:
                    cameraInfo.renderer = std::make_shared<gfx::PbrRenderer>();
                    break;
                default:
                    LOG_WARN("sensor::Manager", "Invalid camera renderer type $0 for entity $1", (int)cameraInfo.component->rendererType,
                             cameraInfo.entity);
            }
            cameraInfo.renderer->setRenderDrawer(false);
            cameraInfo.renderer->setRenderSelected(false);
        }

        // Update camera width/height
        cameraInfo.renderer->resize(cameraInfo.component->width, cameraInfo.component->height);

        // TODO camera projection
    } else {
        LOG_WARN("sensor::Manager", "The camera entity must have a transform component to be rendered");
    }
}

void* Manager::getEntityCameraImGuiTextureImpl(cmp::Entity eid) {
    for (size_t i = 0; i < _cameras.size(); i++)
        if (_cameras[i].entity == eid)
            return _cameras[i].renderer->getImGuiTexture();
    return nullptr;
}

void Manager::cameraCheckUiEvents(event::Event& event) {
    switch (event.getType()) {
        case event::UiCameraComponent::type: {
            event::UiCameraComponent& e = reinterpret_cast<event::UiCameraComponent&>(event);
            for (auto& camera : _cameras)
                if (camera.component == e.component) {
                    camera.showWindow = true;
                    break;
                }
            break;
        }
    }
}

} // namespace atta::sensor
