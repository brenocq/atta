//--------------------------------------------------
// Robot Simulator
// miniCleaner.cpp
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "miniCleaner.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/objects/others/display/display.h"
#include "simulator/objects/sensors/camera/camera.h"
#include "simulator/physics/constraints/constraints.h"
#include "simulator/helpers/log.h"

MiniCleaner::MiniCleaner(glm::vec3 position, glm::vec3 rotation):
	Object("MiniCleaner", position, rotation, {1,1,1}, 0.1f)
{
	_type = "MiniCleaner";
	_model = new Model("box");
	_physics = new ObjectPhysics(_position, _rotation, _mass);

	Box* body = new Box("Body", {0,0,0}, {0,0,0}, {0.18, 0.01,0.05}, 0.1f, {0,0.8,0.8});
	ImportedObject* wheelL = new ImportedObject("Wheel left", "wheel", {0.0,0.0,0}, {0,0,0}, {1,1,1}, 0.1f);
	ImportedObject* wheelR = new ImportedObject("Wheel right", "wheel", {0.0,0.0,0}, {0,0,0}, {1,1,1}, 0.1f);
	Display* display = new Display("Display", {0.0, 0.0, 0.0}, {0,0,0}, {1, 1, 1}, {200,200});

	Camera::CameraInfo camInfo;
	//camInfo.renderingType = Camera::CameraRenderingType::RAY_TRACING;
	camInfo.renderingType = Camera::CameraRenderingType::RASTERIZATION;
	camInfo.width = 200;
	camInfo.height = 200;
	camInfo.fov = 30.0f;
	Camera* camera = new Camera("Camera", {0.0, 0.2, 0.0}, {0,0,0}, camInfo);

	Object::addChild(body, new FixedConstraint({0,0,0}, {0,0,0}));
	Object::addChild(wheelL, new HingeConstraint({0.1,0,0}, {0,0,90}));
	Object::addChild(wheelR, new HingeConstraint({-0.1,0,0}, {0,0,-90}));
	Object::addChild(display, new FixedConstraint({0,0.05,0.05}, {45,0,0}));
	Object::addChild(camera, new FixedConstraint({0,0,0}, {0,0,0}));
}

MiniCleaner::~MiniCleaner()
{
}

void MiniCleaner::run(float dt)
{
	glm::vec3 pos = Object::getPosition();
	pos.x+=.1*dt;
	Object::setPosition(pos);
}
