//--------------------------------------------------
// Atta Graphics
// mesh.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CORE_MESH_H
#define ATTA_GRAPHICS_CORE_MESH_H

#include <string>
#include <vector>
#include <atta/graphics/core/vertex.h>

namespace atta
{
	class Mesh
	{
		public:
			Mesh(std::string meshName);
			~Mesh();

			//---------- Getters ----------//
			std::string getMeshName() const { return _meshName; }
			const std::vector<Vertex>& getVertices() const { return _vertices; }
			const std::vector<uint32_t>& getIndices() const { return _indices; }
			std::vector<std::string> getMaterialNames() const { return _materialNames; }
			unsigned getVerticesSize() const { return _vertices.size(); }
			unsigned getIndicesSize() const { return _indices.size(); }
			unsigned getVerticesOffset() const { return _verticesOffset; }
			unsigned getIndicesOffset() const { return _indicesOffset; }
			unsigned getIndex() const { return _index; }

			//---------- Setters ----------//
			void setVerticesOffset(unsigned verticesOffset) { _verticesOffset = verticesOffset; }
			void setIndicesOffset(unsigned indicesOffset) { _indicesOffset = indicesOffset; }

			static std::vector<Mesh*> allMeshes;

		private:
			void loadMesh();
			void generateBoxMesh();
			void generateCylinderMesh();
			void generatePlaneMesh();
			void generateSphereMesh();

			std::string _meshName;
			unsigned _index;
			std::vector<Vertex> _vertices;
			std::vector<uint32_t> _indices;
			std::vector<std::string> _materialNames;
			unsigned _verticesOffset, _indicesOffset;// TODO Remove them
	};
}

#endif// ATTA_GRAPHICS_CORE_MESH_H
