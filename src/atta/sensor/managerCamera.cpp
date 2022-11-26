//--------------------------------------------------
// Atta Sensor Module
// managerCamera.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::sensor {

void Manager::registerCameras() {
    for (component::EntityId entity : component::getEntitiesView()) {
        component::Camera* camera = component::getComponent<component::Camera>(entity);

        if (camera) {
            // Check if camera was not registered yet
            bool found = false;
            for (auto ci : _cameras)
                if (ci.entity == entity) {
                    found = true;
                    break;
                }

            if (!found) {
                registerCamera(entity, camera);
            }
        }
    }
}

void Manager::registerCamera(component::EntityId entity, component::Camera* camera) {
    // Do not register prototype cameras
    if(component::Entity(entity).isPrototype())
        return;

    CameraInfo cameraInfo{};
    cameraInfo.entity = entity;
    cameraInfo.component = camera;
    cameraInfo.showWindow = false;
    cameraInfo.initialized = false;
    _cameras.push_back(cameraInfo);
}

void Manager::unregisterCameras() { _cameras.clear(); }

void Manager::unregisterCamera(component::EntityId entity) {
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
    for (size_t i = 0; i < _cameras.size(); i++) {
        // Initialize if first time
        if (!_cameras[i].initialized)
            initializeCamera(_cameras[i]);

        // Render if necessary
        float change = Config::getTime() - _cameras[i].lastTime;
        float interval = 1.0f / _cameras[i].component->fps;
        if (change >= interval) {
            updateCameraModel(_cameras[i]);
            _cameras[i].renderer->render(_cameras[i].camera);
            _cameras[i].data = _cameras[i].renderer->getFramebuffer()->readImage(0);
            _cameras[i].lastTime = Config::getTime();
        }
    }
}

void Manager::initializeCamera(CameraInfo& cameraInfo) {
    // Need to make sure to initialize after the graphics module has started and it is not in the middle of UI rendering
    component::Camera* camera = component::getComponent<component::Camera>(cameraInfo.entity);

    // Start with random last time (used to distribute camera rendering across time)
    cameraInfo.lastTime = 0.0f;//(rand() % (int)(10000.0f / camera->fps)) / 10000.0f;

    // Create renderer
    switch (camera->rendererType) {
        case component::Camera::RendererType::FAST:
            cameraInfo.renderer = std::make_shared<graphics::FastRenderer>();
            break;
        case component::Camera::RendererType::PHONG:
            cameraInfo.renderer = std::make_shared<graphics::PhongRenderer>();
            break;
        case component::Camera::RendererType::PBR:
            cameraInfo.renderer = std::make_shared<graphics::PbrRenderer>();
            break;
        default:
            LOG_WARN("sensor::Manager", "Invalid camera renderer type $0 for entity $1", (int)camera->rendererType, cameraInfo.entity);
    }
    cameraInfo.renderer->setRenderDrawer(false);
    cameraInfo.renderer->setRenderSelected(false);
    cameraInfo.renderer->resize(camera->width, camera->height);

    // Create camera (view/projection matrices)
    switch (camera->cameraType) {
        case component::Camera::CameraType::ORTHOGRAPHIC: {
            graphics::OrthographicCamera::CreateInfo info{};
            info.height = camera->fov; // TODO union
            info.far = camera->far;
            info.ratio = camera->width / (float)camera->height;

            cameraInfo.camera = std::static_pointer_cast<graphics::Camera>(std::make_shared<graphics::OrthographicCamera>(info));
            break;
        }
        case component::Camera::CameraType::PERSPECTIVE: {
            graphics::PerspectiveCamera::CreateInfo info{};
            info.fov = camera->fov;
            info.far = camera->far;
            info.near = camera->near;
            info.ratio = camera->width / (float)camera->height;
            cameraInfo.camera = std::static_pointer_cast<graphics::Camera>(std::make_shared<graphics::PerspectiveCamera>(info));
            break;
        }
        default:
            LOG_WARN("sensor::Manager", "Invalid camera projection type $0 for entity $1", (int)camera->cameraType, cameraInfo.entity);
    }
    cameraInfo.initialized = true;
}

void Manager::updateCameraModel(CameraInfo& cameraInfo) {
    //----- Update camera pose and parameters -----//
    component::EntityId entity = cameraInfo.entity;
    component::Transform* transform = component::getComponent<component::Transform>(entity);
    component::Relationship* relationship = component::getComponent<component::Relationship>(entity);
    if (transform) {
        // Calculate position
        vec3 position;
        quat ori;
        vec3 scale;
        mat4 worldModel = transform->getWorldTransform(entity);
        worldModel.getPosOriScale(position, ori, scale);

        // Calculate rotation
        mat4 rotation;
        rotation.setPosOriScale(transform->position, transform->orientation, transform->scale);
        rotation.mat[0][3] = 0.0f;
        rotation.mat[1][3] = 0.0f;
        rotation.mat[2][3] = 0.0f;
        rotation.mat[0][0] /= transform->scale.x;
        rotation.mat[1][1] /= transform->scale.y;
        rotation.mat[2][2] /= transform->scale.z;
        while (relationship && relationship->getParent() >= 0) {
            component::Transform* ptransform = component::getComponent<component::Transform>(relationship->getParent());
            if (ptransform) {
                mat4 protation;
                protation.setPosOriScale(ptransform->position, ptransform->orientation, ptransform->scale);
                protation.mat[0][3] = 0.0f;
                protation.mat[1][3] = 0.0f;
                protation.mat[2][3] = 0.0f;
                protation.mat[0][0] /= ptransform->scale.x;
                protation.mat[1][1] /= ptransform->scale.y;
                protation.mat[2][2] /= ptransform->scale.z;
                rotation = protation * rotation;
            }
            relationship = component::getComponent<component::Relationship>(relationship->getParent());
        }

        // Update camera pose
        cameraInfo.camera->setPosition(position);
        cameraInfo.camera->setFront(rotation * vec3(0, 0, 1));
        cameraInfo.camera->setUp(rotation * vec3(1, 0, 0));
        cameraInfo.camera->update();

        // Update camera fov
        if (cameraInfo.component->cameraType == component::Camera::CameraType::PERSPECTIVE) {
            std::shared_ptr<graphics::PerspectiveCamera> persCam = std::static_pointer_cast<graphics::PerspectiveCamera>(cameraInfo.camera);
            persCam->setFov(radians(cameraInfo.component->fov));
        }

        // Update renderer
        component::Camera::RendererType currRendererType;
        if (cameraInfo.renderer->getName() == "FastRenderer")
            currRendererType = component::Camera::RendererType::FAST;
        else if (cameraInfo.renderer->getName() == "PhongRenderer")
            currRendererType = component::Camera::RendererType::PHONG;
        else if (cameraInfo.renderer->getName() == "PbrRenderer")
            currRendererType = component::Camera::RendererType::PBR;

        if (cameraInfo.component->rendererType != currRendererType) {
            switch (cameraInfo.component->rendererType) {
                case component::Camera::RendererType::FAST:
                    cameraInfo.renderer = std::make_shared<graphics::FastRenderer>();
                    break;
                case component::Camera::RendererType::PHONG:
                    cameraInfo.renderer = std::make_shared<graphics::PhongRenderer>();
                    break;
                case component::Camera::RendererType::PBR:
                    cameraInfo.renderer = std::make_shared<graphics::PbrRenderer>();
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

void* Manager::getEntityCameraImGuiTextureImpl(component::EntityId eid) {
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
