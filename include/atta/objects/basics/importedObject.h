//--------------------------------------------------
// Robot Simulator
// importedObject.h
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H
#define ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H

#include <atta/objects/object.h>

namespace atta
{
	class ImportedObject : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "ImportedObject";
				std::string fileName = "";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;
				Material material;
			};

			ImportedObject(CreateInfo info);
			~ImportedObject();

		private:
	};
}

#endif// ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H
