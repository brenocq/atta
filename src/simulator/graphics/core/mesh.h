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

			//---------- Getters ----------//
			std::string getMeshName() const { return _meshName; }
			const std::vector<Vertex>& getVertices() const { return _vertices; }
			const std::vector<uint32_t>& getIndices() const { return _indices; }
			unsigned getVerticesSize() const { return _vertices.size(); }
			unsigned getIndicesSize() const { return _indices.size(); }
			unsigned getVerticesOffset() const { return _verticesOffset; }
			unsigned getIndicesOffset() const { return _indicesOffset; }

			//---------- Setters ----------//
			void setVerticesOffset(unsigned verticesOffset) { _verticesOffset = verticesOffset; }
			void setIndicesOffset(unsigned indicesOffset) { _indicesOffset = indicesOffset; }

		private:
			void loadMesh();
			void generateBoxMesh();

			std::string _meshName;
			std::vector<Vertex> _vertices;
			std::vector<uint32_t> _indices;
			unsigned _verticesOffset, _indicesOffset;
	};
}

#endif// ATTA_GRAPHICS_MESH_H