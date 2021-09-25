//--------------------------------------------------
// Atta Sensor System
// sensorManager.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/sensorSystem/sensorManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/relationshipComponent.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>
#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>

namespace atta
{
	SensorManager& SensorManager::getInstance()
	{
		static SensorManager instance;
		return instance;
	}

	void SensorManager::startUp() { getInstance().startUpImpl(); }
	void SensorManager::startUpImpl()
	{
		EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(SensorManager::onSimulationStateChange));
		EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(SensorManager::onSimulationStateChange));
	}

	void SensorManager::shutDown() { getInstance().shutDownImpl(); }
	void SensorManager::shutDownImpl()
	{
	}

	void SensorManager::update(float dt) { getInstance().updateImpl(dt); }
	void SensorManager::updateImpl(float dt)
	{
		_currTime += dt;
		cameraUpdate(dt);
	}

	void SensorManager::onSimulationStateChange(Event& event)
	{
		switch(event.getType())
		{
			case SimulationStartEvent::type:
				{
					_currTime = 0;
					cameraStart();
					break;
				}
			case SimulationStopEvent::type:
				{
					_currTime = 0;
					cameraStop();
					break;
				}
			default:
				{
					LOG_WARN("SensorManager", "Unknown simulation event");
				}
		}
	}

	void SensorManager::cameraStart()
	{
		for(auto entity : ComponentManager::getEntities())
		{
			CameraComponent* camera = ComponentManager::getEntityComponent<CameraComponent>(entity);
			if(camera)
			{
				_cameraEntities.push_back({entity, camera});
				// Start with random last time (used to distribute camera rendering across time)
				_cameraLastTime.push_back((rand()%(int)(10000.0f/camera->fps))/10000.0f);

				switch(camera->rendererType)
				{
					case CameraComponent::RendererType::FAST:
						_cameraRenderers.push_back(std::make_shared<FastRenderer>());
						break;
					case CameraComponent::RendererType::PHONG:
						_cameraRenderers.push_back(std::make_shared<PhongRenderer>());
						break;
					case CameraComponent::RendererType::PBR:
						_cameraRenderers.push_back(std::make_shared<PbrRenderer>());
						break;
					default:
						LOG_WARN("SensorManager", "Invalid camera renderer type $0 for entity $1", (int)camera->rendererType, entity);
				}
				_cameraRenderers.back()->resize(camera->width, camera->height);

				switch(camera->cameraType)
				{
					case CameraComponent::CameraType::ORTHOGRAPHIC:
					{
						OrthographicCamera::CreateInfo info {};
						info.height = camera->fov;// TODO union
						info.far = camera->far;
						info.ratio = camera->width/(float)camera->height;

						_cameraCameras.push_back(
								std::static_pointer_cast<Camera>(
									std::make_shared<OrthographicCamera>(info)));
						break;
					}
					case CameraComponent::CameraType::PERSPECTIVE:
					{
						PerspectiveCamera::CreateInfo info {};
						info.fov = camera->fov;
						info.far = camera->far;
						info.near = camera->near;
						info.ratio = camera->width/(float)camera->height;
						_cameraCameras.push_back(
								std::static_pointer_cast<Camera>(
									std::make_shared<PerspectiveCamera>(info)));
						break;
					}
					default:
						LOG_WARN("SensorManager", "Invalid camera projection type $0 for entity $1", (int)camera->cameraType, entity);
				}
			}
		}

		LOG_DEBUG("SensorManager", "Found $0 camera components. Time: $1", _cameraEntities.size(), _cameraLastTime);
	}

	void SensorManager::cameraUpdate(float dt)
	{
		for(size_t i = 0; i < _cameraLastTime.size(); i++)
		{
			float change = _currTime - _cameraLastTime[i];
			if(change >= 1.0f/_cameraEntities[i].second->fps)
			{
				//LOG_DEBUG("SensorManager", "Render camera $0", _cameraEntities[i].first);
				EntityId entity = _cameraEntities[i].first;
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
				RelationshipComponent* relationship = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
				if(transform)
				{
					mat4 model; 
					model.setPosOriScale(transform->position, transform->orientation, transform->scale);

					while(relationship && relationship->parent >= 0)
					{
						TransformComponent* ptransform = ComponentManager::getEntityComponent<TransformComponent>(relationship->parent);
						if(ptransform)
						{
							mat4 pmodel; 
							pmodel.setPosOriScale(ptransform->position, ptransform->orientation, ptransform->scale);
							model = pmodel * model;
						}
						relationship = ComponentManager::getEntityComponent<RelationshipComponent>(relationship->parent);
					}

					vec3 position;
					quat ori;
					vec3 scale;
					model.getPosOriScale(position, ori, scale);

					mat4 rotation; 
					rotation.setPosOriScale(transform->position, transform->orientation, transform->scale);
					rotation.mat[0][3] = 0.0f;
					rotation.mat[1][3] = 0.0f;
					rotation.mat[2][3] = 0.0f;
					rotation.mat[0][0] /= transform->scale.x;
					rotation.mat[1][1] /= transform->scale.y;
					rotation.mat[2][2] /= transform->scale.z;
					while(relationship && relationship->parent >= 0)
					{
						TransformComponent* ptransform = ComponentManager::getEntityComponent<TransformComponent>(relationship->parent);
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
						relationship = ComponentManager::getEntityComponent<RelationshipComponent>(relationship->parent);
					}

					_cameraCameras[i]->setPosition(position);
					_cameraCameras[i]->setFront(rotation*vec3(0,0,1));
					_cameraCameras[i]->setUp(rotation*vec3(1,0,0));
					_cameraCameras[i]->update();
					_cameraRenderers[i]->render(_cameraCameras[i]);
				}
				else
				{
					LOG_WARN("SensorManager", "The camera entity must have a transform component to be rendered");
				}
			}
		}
	}

	void SensorManager::cameraStop()
	{
		_cameraEntities.clear();
		_cameraLastTime.clear();
		_cameraRenderers.clear();
	}

	void* SensorManager::getEntityCameraImGuiTexture(EntityId eid) { return getInstance().getEntityCameraImGuiTextureImpl(eid); }
	void* SensorManager::getEntityCameraImGuiTextureImpl(EntityId eid)
	{
		for(size_t i = 0; i < _cameraEntities.size(); i++)
		{
			if(_cameraEntities[i].first == eid)
			{
				return _cameraRenderers[i]->getImGuiTexture();
			}
		}
		return nullptr;
	}
}
