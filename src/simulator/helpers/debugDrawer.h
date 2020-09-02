//--------------------------------------------------
// Robot Simulator
// debugDrawer.h
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DRAW_HELPER_H
#define DRAW_HELPER_H

#include "../scene.h"
#include "LinearMath/btIDebugDraw.h"

// TODO implementing bullet debug drawer, should change this later to support new types of debugging
class DebugDrawer : public btIDebugDraw
{
	public:
		DebugDrawer(Scene* scene);
		~DebugDrawer();

		// Debug mode functions
		virtual void setDebugMode(int debugMode) override { _debugMode = debugMode; }
		virtual int getDebugMode() const override { return _debugMode; }

		// Drawing functions
		virtual void  drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
		virtual void  drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

		// Unused
		virtual void  reportErrorWarning(const char* warningString) override {}
		virtual void  draw3dText(const btVector3 &location,const char* textString) override {}

		void toggleDebugFlag(int flag);

	private:
		Scene* _scene;
		int _debugMode;
};

#endif// DRAW_HELPER_H
