//--------------------------------------------------
// Atta
// drawer.h
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_DRAWER_H
#define ATTA_DRAWER_H

#include "../scene.h"

namespace atta
{
	// Add lines to the scene
	class Drawer
	{
		public:
			Drawer(Scene* scene);
			~Drawer();

			void drawPhysicsShapes();

			void drawSphere(vec3 position, quat orientation, float radius, vec3 color);
			void drawBox(vec3 position, quat orientation, vec3 size, vec3 color);
			void drawCylinder(vec3 position, quat orientation, vec3 size, vec3 color);
			void drawCapsule(vec3 position, quat orientation, vec3 size, vec3 color);
		private:
			Scene* _scene;
	};
}
#endif// ATTA_DRAWER_H
