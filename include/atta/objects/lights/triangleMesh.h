//--------------------------------------------------
// Robot Simulator
// triangleMesh.h
// Date: 2021-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_LIGHTS_TRIANGLE_MESH_LIGHT_H
#define ATTA_OBJECTS_LIGHTS_TRIANGLE_MESH_LIGHT_H

#include <atta/objects/object.h>

namespace atta
{
	class TriangleMeshLight : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "TriangleMeshLight";
				vec3 radiance = {1,1,1};// Light intensity (amount of power per unit solid angle)
				std::vector<vec3> vertices;// One triangle for each 3 vertices
			};

			TriangleMeshLight(CreateInfo info);
			~TriangleMeshLight();

			//---------- Getters ----------//
			vec3 getRadiance() const { return _radiance; }
			std::vector<vec3> getVertices() const { return _vertices; }

		private:
			vec3 _radiance;
			std::vector<vec3> _vertices;
	};
}

#endif// ATTA_OBJECTS_LIGHTS_TRIANGLE_MESH_LIGHT_H
