//--------------------------------------------------
// Robot Simulator
// box.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BOX_H
#define BOX_H

#include "simulator/objects/object.h"

namespace atta
{
	class Box : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "Box";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;
				Material material;
			};

			Box(CreateInfo info);
			~Box();

		private:
	};
}

#endif// BOX_H
