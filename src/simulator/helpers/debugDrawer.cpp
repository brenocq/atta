//--------------------------------------------------
// Robot Simulator
// debugDrawer.cpp
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "debugDrawer.h"

DebugDrawer::DebugDrawer(Scene* scene)
{
	_scene = scene;
	setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	_scene->getPhysicsEngine()->getWorld()->setDebugDrawer(this);
}

DebugDrawer::~DebugDrawer()
{

}

void DebugDrawer::drawLine(const btVector3 &from,const btVector3 &to, const btVector3 &color)
{
	// Draws a simple line of pixels between points.
	_scene->addLine( PhysicsEngine::bt2glm(from), PhysicsEngine::bt2glm(to), PhysicsEngine::bt2glm(color) );
}

void DebugDrawer::drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
	// draws a line between two contact points
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	_scene->addLine( PhysicsEngine::bt2glm(startPoint), PhysicsEngine::bt2glm(endPoint), PhysicsEngine::bt2glm(color) );
}

void DebugDrawer::toggleDebugFlag(int flag)
{
	// Checks if a flag is set and enables/
	// Disables it
	if (_debugMode & flag)
		// Dlag is enabled, so disable it
		_debugMode = _debugMode & (~flag);
	else
		// Flag is disabled, so enable it
		_debugMode |= flag;
}

