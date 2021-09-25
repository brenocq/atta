//--------------------------------------------------
// Atta Sensor System
// sensorManager.h
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SENSOR_SYSTEM_SENSOR_MANAGER_H
#define ATTA_SENSOR_SYSTEM_SENSOR_MANAGER_H
#include <atta/componentSystem/base.h>
#include <atta/componentSystem/components/cameraComponent.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
	class SensorManager final
	{
	public:
		static SensorManager& getInstance();
		static void startUp();
		static void shutDown();

		static void update(float dt);

		static void* getEntityCameraImGuiTexture(EntityId eid);

	private:
		void startUpImpl();
		void shutDownImpl();
		void updateImpl(float dt);
		void* getEntityCameraImGuiTextureImpl(EntityId eid);

		// Handle events
		void onSimulationStateChange(Event& event);

		// Camera
		void cameraStart();
		void cameraUpdate(float dt);
		void cameraStop();

		float _currTime;
		//----- Camera -----//
		std::vector<std::pair<EntityId, CameraComponent*>> _cameraEntities;
		std::vector<std::shared_ptr<Renderer>> _cameraRenderers;
		std::vector<std::shared_ptr<Camera>> _cameraCameras;
		std::vector<float> _cameraLastTime;// Used to render with the correct fps
		uint8_t** _camerasData;
	};
}

#endif// ATTA_SENSOR_SYSTEM_SENSOR_MANAGER_H
