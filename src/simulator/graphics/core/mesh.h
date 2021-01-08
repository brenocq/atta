//--------------------------------------------------
// Atta Graphics
// mesh.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MESH_H
#define ATTA_GRAPHICS_MESH_H

#include <string>
#include <vector>
#include "vertex.h"

namespace atta
{
	class Mesh
	{
		public:
			Mesh(std::string meshName);
			~Mesh();

		private:
			void loadMesh();
			void generateBoxMesh();

			std::string _meshName;
			std::vector<Vertex> _vertices;
			std::vector<uint32_t> _indices;
	};
}

#endif// ATTA_GRAPHICS_MESH_H
