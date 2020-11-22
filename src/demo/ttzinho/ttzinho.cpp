//--------------------------------------------------
// Robot Simulator
// ttzinho.cpp
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "ttzinho.h"
#include "simulator/objects/basic/box.h"
#include "simulator/objects/basic/importedObject.h"
#include "simulator/objects/others/display/display.h"
#include "simulator/objects/sensors/camera/camera.h"
#include "simulator/physics/constraints/hingeConstraint.h"
#include "simulator/helpers/log.h"

Ttzinho::Ttzinho()
{
	_object = new Box("Ttzinho", {0,0.06,0}, {0,0,0}, {0.18, 0.01,0.05}, 0.1f, {0,0,0.8});
	ImportedObject* wheelL = new ImportedObject("Wheel left", "wheel", {0.2,0.06,0}, {0,0,90}, {1,1,1}, 0.1f);
	ImportedObject* wheelR = new ImportedObject("Wheel right", "wheel", {0.2,0.12,0}, {0,0,-90}, {1,1,1}, 0.1f);
	Display* display = new Display("Display", {0.0, 2.0, 0.0}, {0,0,0}, {200,200});

	Camera::CameraInfo camInfo;
	camInfo.renderingType = Camera::CameraRenderingType::RAY_TRACING;
	camInfo.width = 200;
	camInfo.height = 200;
	camInfo.fov = 30.0f;
	Camera* camera = new Camera("Camera", {0.0, 0.2, 0.0}, {0,0,0}, camInfo);

	_object->addChild(wheelL, new HingeConstraint({0.2,0,0}, {0,0,90}));
	_object->addChild(wheelR, new HingeConstraint({-0.2,0,0}, {0,0,-90}));
	_object->addChild(display, new HingeConstraint({-0.2,0,0}, {0,0,-90}));
	_object->addChild(camera, new HingeConstraint({-0.2,0,0}, {0,0,-90}));
}

Ttzinho::~Ttzinho()
{
	if(_object != nullptr)
	{
		delete _object;
		_object = nullptr;
	}
}
