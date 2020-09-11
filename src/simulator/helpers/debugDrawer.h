//--------------------------------------------------
// Robot Simulator
// debugDrawer.h
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DRAW_HELPER_H
#define DRAW_HELPER_H

#include "../scene.h"

// TODO implement physics debug drawer, should change this later to support new types of debugging
class DebugDrawer
{
	public:
		DebugDrawer(Scene* scene);
		~DebugDrawer();

	private:
		Scene* _scene;
		int _debugMode;
};

#endif// DRAW_HELPER_H
