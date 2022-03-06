//--------------------------------------------------
// Atta Sensor System
// sensorManager.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    void SensorManager::registerCameras()
    {
        std::unordered_set<EntityId> camerasToDelete;// Start with all cameras and 
        for(auto ci : _cameras)
            camerasToDelete.insert(ci.entity);

        // Add new cameras
        for(EntityId entity : ComponentManager::getEntitiesView())
        {
            CameraComponent* camera = ComponentManager::getEntityComponent<CameraComponent>(entity);

            if(camera)
            {
                // Check if camera was not registered yet
                bool found = false;
                for(auto ci : _cameras)
                    if(ci.entity == entity)
                    {
                        found = true;
                        camerasToDelete.erase(entity);
                        break;
                    }

                if(!found)
                    registerCamera(entity, camera);
            }
        }

        // Remove deleted cameras
        for(EntityId cameraEntity : camerasToDelete)
        {
            for(unsigned i = 0; i < _cameras.size(); i++)
                if(_cameras[i].entity == cameraEntity)
                {
                    _cameras.erase(_cameras.begin()+i);
                    break;
                }
        }
    }

    void SensorManager::unregisterCameras()
    {
        _cameras.clear();
    }

    void SensorManager::registerCamera(EntityId entity, CameraComponent* camera)
    {
        CameraInfo cameraInfo;
        cameraInfo.entity = entity;
        cameraInfo.component = camera;
        cameraInfo.showWindow = false;

        // Start with random last time (used to distribute camera rendering across time)
        cameraInfo.lastTime = (rand()%(int)(10000.0f/camera->fps))/10000.0f;

        // Create renderer
        switch(camera->rendererType)
        {
            case CameraComponent::RendererType::FAST:
                LOG_VERBOSE("SensorManager", "$0 is FAST", entity);
                cameraInfo.renderer = std::make_shared<FastRenderer>();
                break;
            case CameraComponent::RendererType::PHONG:
                LOG_VERBOSE("SensorManager", "$0 is PHONG", entity);
                cameraInfo.renderer = std::make_shared<PhongRenderer>();
                break;
            case CameraComponent::RendererType::PBR:
                LOG_VERBOSE("SensorManager", "$0 is PBR", entity);
                cameraInfo.renderer = std::make_shared<PbrRenderer>();
            break;
            default:
                LOG_WARN("SensorManager", "Invalid camera renderer type $0 for entity $1", (int)camera->rendererType, entity);
        }
        cameraInfo.renderer->setRenderDrawer(false);
        cameraInfo.renderer->setRenderSelected(false);
        cameraInfo.renderer->resize(camera->width, camera->height);

        // Create camera (view/projection matrices)
        switch(camera->cameraType)
        {
            case CameraComponent::CameraType::ORTHOGRAPHIC:
                {
                    OrthographicCamera::CreateInfo info {};
                    info.height = camera->fov;// TODO union
                    info.far = camera->far;
                    info.ratio = camera->width/(float)camera->height;

                    cameraInfo.camera = std::static_pointer_cast<Camera>(
                                std::make_shared<OrthographicCamera>(info));
                    break;
                }
            case CameraComponent::CameraType::PERSPECTIVE:
                {
                    PerspectiveCamera::CreateInfo info {};
                    info.fov = camera->fov;
                    info.far = camera->far;
                    info.near = camera->near;
                    info.ratio = camera->width/(float)camera->height;
                    cameraInfo.camera = std::static_pointer_cast<Camera>(
                                std::make_shared<PerspectiveCamera>(info));
                    break;
                }
            default:
                LOG_WARN("SensorManager", "Invalid camera projection type $0 for entity $1", (int)camera->cameraType, entity);
        }

        _cameras.push_back(cameraInfo);
    }

    void SensorManager::unregisterCamera(EntityId entity)
    {
        bool found = false;
        for(int i = _cameras.size()-1; i >= 0; i--)
        {
            if(_cameras[i].entity == entity)
            {
                _cameras.erase(_cameras.begin()+i);
                found = true;
                break;
            }
        }

        if(!found)
            LOG_WARN("SensorManager", "Could not unregister camera from entity [w]$0[], camera was not registered before", entity);
    }

    void SensorManager::updateCamerasModel()
    {
        //----- Update camera pose and parameters -----//
        for(size_t i = 0; i < _cameras.size(); i++)
        {
            EntityId entity = _cameras[i].entity;
            TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
            RelationshipComponent* relationship = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
            if(transform)
            {
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
                while(relationship && relationship->getParent() >= 0)
                {
                    TransformComponent* ptransform = ComponentManager::getEntityComponent<TransformComponent>(relationship->getParent());
                    if(ptransform)
                    {
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
                    relationship = ComponentManager::getEntityComponent<RelationshipComponent>(relationship->getParent());
                }

                // Update camera pose
                _cameras[i].camera->setPosition(position);
                _cameras[i].camera->setFront(rotation*vec3(0,0,1));
                _cameras[i].camera->setUp(rotation*vec3(1,0,0));
                _cameras[i].camera->update();

                // Update camera fov
                if(_cameras[i].component->cameraType == CameraComponent::CameraType::PERSPECTIVE)
                {
                    std::shared_ptr<PerspectiveCamera> persCam = std::static_pointer_cast<PerspectiveCamera>(_cameras[i].camera);
                    persCam->setFov(radians(_cameras[i].component->fov));
                }

                // TODO camera width/height

                // Update renderer
                CameraComponent::RendererType currRendererType;
                if(_cameras[i].renderer->getName() == "FastRenderer")
                    currRendererType = CameraComponent::RendererType::FAST;
                else if(_cameras[i].renderer->getName() == "PhongRenderer")
                    currRendererType = CameraComponent::RendererType::PHONG;
                else if(_cameras[i].renderer->getName() == "PbrRenderer")
                    currRendererType = CameraComponent::RendererType::PBR;

                if(_cameras[i].component->rendererType != currRendererType)
                {
                    switch(_cameras[i].component->rendererType)
                    {
                        case CameraComponent::RendererType::FAST:
                            LOG_VERBOSE("SensorManager", "up $0 is FAST", entity);
                            _cameras[i].renderer = std::make_shared<FastRenderer>();
                            break;
                        case CameraComponent::RendererType::PHONG:
                            LOG_VERBOSE("SensorManager", "up $0 is PHONG", entity);
                            _cameras[i].renderer = std::make_shared<PhongRenderer>();
                            break;
                        case CameraComponent::RendererType::PBR:
                            LOG_VERBOSE("SensorManager", "up $0 is PBR", entity);
                            _cameras[i].renderer = std::make_shared<PbrRenderer>();
                        break;
                        default:
                            LOG_WARN("SensorManager", "Invalid camera renderer type $0 for entity $1", (int)_cameras[i].component->rendererType, _cameras[i].entity);
                    }
                    _cameras[i].renderer->setRenderDrawer(false);
                    _cameras[i].renderer->setRenderSelected(false);
                    _cameras[i].renderer->resize(_cameras[i].component->width, _cameras[i].component->height);
                }
                
                // TODO camera projection
            }
            else
            {
                LOG_WARN("SensorManager", "The camera entity must have a transform component to be rendered");
            }
        }
    }

    void SensorManager::updateCameras(float dt)
    {
        for(size_t i = 0; i < _cameras.size(); i++)
        {
            // Render if necessary
            float change = _currTime - _cameras[i].lastTime;
            if(change >= 1.0f/_cameras[i].component->fps)
            {
                _cameras[i].renderer->render(_cameras[i].camera);
                _cameras[i].data = _cameras[i].renderer->getFramebuffer()->readImage(0);
            }
        }
    }

    void* SensorManager::getEntityCameraImGuiTexture(EntityId eid) { return getInstance().getEntityCameraImGuiTextureImpl(eid); }
    void* SensorManager::getEntityCameraImGuiTextureImpl(EntityId eid)
    {
        for(size_t i = 0; i < _cameras.size(); i++)
            if(_cameras[i].entity == eid)
                return _cameras[i].renderer->getImGuiTexture();
        return nullptr;
    }

    void SensorManager::cameraCheckUIEvents(Event& event)
    {
        switch(event.getType())
        {
            case UICameraComponentEvent::type:
                {
                    UICameraComponentEvent& e = reinterpret_cast<UICameraComponentEvent&>(event);
                    for(auto& camera : _cameras)
                        if(camera.component == e.component)
                        {
                            camera.showWindow = true;
                            break;
                        }
                    break;
                }
        }
    }

    std::vector<SensorManager::CameraInfo>& SensorManager::getCameraInfos() { return getInstance()._cameras; }
}
