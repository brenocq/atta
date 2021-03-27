//--------------------------------------------------
// Robot Simulator
// importedObject.h
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H
#define ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H

#include <atta/objects/object.h>
#include <vector>
#include <memory>
#include <map>

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

				// Only material or materials show be used
				// .material should be used to provide only one material to the model
				// .materials should be used when the model have more than one material in the .mtl file
				Material material = {};
				std::map<std::string, Material> materials = {};

				std::vector<std::shared_ptr<Object>> children = {};
			};

			ImportedObject(CreateInfo info);
			~ImportedObject();

		private:
	};
}

#endif// ATTA_OBJECTS_BASICS_IMPORTED_OBJECT_H
