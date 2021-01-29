//--------------------------------------------------
// Robot PhysicsEngine
// physicsEngine.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicsEngine.h"
#include "forces/forces.h"

namespace atta::phy
{
	PhysicsEngine::PhysicsEngine(CreateInfo info):
		_accelerator(info.accelerator)
	{
		_forceGenerator = std::make_shared<ForceGenerator>();
		_contactResolver = std::make_shared<ContactResolver>(100);
		_contactGenerator = std::make_shared<ContactGenerator>();

		// Get bodies from objects
		for(auto object : _accelerator->getObjects())
		{
			if(object->getBodyPhysics())
			{
				_bodies.push_back(object->getBodyPhysics());
			}
		}
	}

	PhysicsEngine::~PhysicsEngine()
	{

	}

	void PhysicsEngine::stepPhysics(float dt)
	{
		//---------- Move objects ----------//
		_forceGenerator->updateForces(dt);
		for(auto body : _bodies)
		{
			body->addForce({0,-9.8,0});
			body->integrate(dt);
		}
		
		//---------- Broad Phase ----------//
		// TODO After updating accelerator tree, get contacts from broadphase

		// Simulating worst case
		std::vector<std::pair<std::shared_ptr<Body>, std::shared_ptr<Body>>> possibleContacts;
		unsigned size = _bodies.size();
		for(unsigned i=0; i<size;i++)
		{
			for(unsigned j=i+1; j<size;j++)
			{
				if(i!=j)
				{
					possibleContacts.push_back(std::make_pair(_bodies[i], _bodies[j]));
				}
			}
		}
		//Log::debug("PhysicsEngine", "BroadPhase: $0", possibleContacts.size());

		//---------- Narrow Phase ----------//
		_contactGenerator->clearContacts();
		for(auto contact : possibleContacts)
		{
			for(auto shape1 : contact.first->getShapes())
			{
				for(auto shape2 : contact.second->getShapes())
				{
					_contactGenerator->testContact(shape1, shape2);
				}
			}
		}
		Log::debug("PhysicsEngine", "Qty contacts: $0", _contactGenerator->qtyContacts());
		
		//---------- Resolve contacts ----------//
	}

	//---------- Static functions ----------//
	//vec3 PhysicsEngine::getMouseClickRay(int x, int y, int width, int height, vec3 camPos, vec3 camForward, vec3 camUp)
	//{
	//	//const float nearPlane = 0.1f;
	//	const float farPlane = 1000.0f;

	//	// Calculate fielf-of-view
	//	float tanFov = 1.0f;//1.0f/nearPlane;
	//	//float fov = 2.0*atan(tanFov);

	//	// Get ray pointing forward from the camera and extend it to the far plane
	//	vec3 rayForward = camForward.unit();
	//	rayForward *= farPlane;

	//	// Find horizontal and vertical vectors relative to the camera
	//	vec3 ver = {0.0f, 1.0f, 0.0f};
	//	vec3 hor = rayForward.cross(ver);
	//	hor.normalize();
	//	ver = hor.cross(rayForward);
	//	ver.normalize();
	//	hor *= 2.f * farPlane * tanFov;
	//	ver *= 2.f * farPlane * tanFov;

	//	// Calculate aspect ratio
	//	float aspect = width/(float)height;

	//	// Adjust forward-ray based on the X/Y coordinates that were clicked
	//	hor *= aspect;
	//	vec3 rayToCenter = camPos + rayForward;
	//	vec3 dHor = hor * 1.f/(float)width;
	//	vec3 dVer = ver * 1.f/(float)height;
	//	vec3 rayTo = rayToCenter - hor*0.5f + ver*0.5f;
	//	rayTo += dHor*float(x);
	//	rayTo -= dVer*float(y);
	//		
	//	return rayTo;
	//}
}
