//--------------------------------------------------
// Robot Simulator
// halfSpace.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_HALF_SPACE_H
#define ATTA_OBJECTS_BASICS_HALF_SPACE_H

#include <atta/objects/object.h>

namespace atta
{
	class HalfSpace : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "HalfSpace";
				vec3 normal = {0,1,0};
				float offset = 0;
				Material material;
			};

			HalfSpace(CreateInfo info);
			~HalfSpace();

			//---------- Getters ----------//
			vec3 getColor() const { return _color; };

		private:
			vec3 _color;
	};
}

#endif// ATTA_OBJECTS_BASICS_HALF_SPACE_H
