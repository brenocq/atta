//--------------------------------------------------
// Atta Graphics
// mesh.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <unordered_map>
#include "mesh.h"
#include "tiny_obj_loader.h"
#include "simulator/helpers/evaluator.h"
#include "simulator/helpers/log.h"

namespace atta
{
	Mesh::Mesh(std::string meshName):
		_meshName(meshName)
	{
		if(meshName.find("atta::") == std::string::npos)
		{
			// Load from file
			loadMesh();
		}
		else
		{
			// Special mesh from atta
			if(meshName == "atta::box")
			{
				generateBoxMesh();
			}
		}
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::loadMesh()
	{
		LocalEvaluator eval;
		std::string objPath = "assets/models/"+_meshName+"/"+_meshName+".obj";

		//---------- Parse file ----------//
		tinyobj::ObjReader objReader;
		if(!objReader.ParseFromFile(objPath))
		{
			Log::error("Mesh", "Failed to load model [w]$0[]: $1", objPath, objReader.Error());
			exit(1);
		}

		if(!objReader.Warning().empty())
		{
			Log::warning("Mesh", "Warning while parsing [w]$0[]: $1", objPath, objReader.Warning());
		}
		//---------- Materials ----------//
		// TODO

		//---------- Geometry ----------//
		const auto& objAttrib = objReader.GetAttrib();

		std::unordered_map<Vertex, uint32_t> uniqueVertices(objAttrib.vertices.size());
		size_t faceId = 0;
		for(const auto& shape : objReader.GetShapes())
		{
			const auto& mesh = shape.mesh;
			for (const auto& index : mesh.indices)
			{
				Vertex vertex = {};
				vertex.pos =
				{
					objAttrib.vertices[3 * index.vertex_index + 0],
					objAttrib.vertices[3 * index.vertex_index + 1],
					objAttrib.vertices[3 * index.vertex_index + 2],
				};

				if (!objAttrib.normals.empty())
				{
					vertex.normal =
					{
						objAttrib.normals[3 * index.normal_index + 0],
						objAttrib.normals[3 * index.normal_index + 1],
						objAttrib.normals[3 * index.normal_index + 2]
					};
				}

				if (!objAttrib.texcoords.empty())
				{
					vertex.texCoord =
					{
						objAttrib.texcoords[2 * index.texcoord_index + 0],
						1 - objAttrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				vertex.materialIndex = std::max(0, mesh.material_ids[faceId++ / 3]);

				if(uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
					_vertices.push_back(vertex);
				}

				_indices.push_back(uniqueVertices[vertex]);
			}
		}

		// If the model did not specify normals, then create smooth normals that conserve the same number of vertices.
		// Using flat normals would mean creating more vertices than we currently have, 
		// so for simplicity and better visuals we don't do it.
		// See https://stackoverflow.com/questions/12139840/obj-file-averaging-normals.
		if(objAttrib.normals.empty())
		{
			std::vector<glm::vec3> normals(_vertices.size());
			
			for(size_t i = 0; i < _indices.size(); i += 3)
			{
				const auto normal = atta::normalize(atta::cross(
					vec3(_vertices[_indices[i + 1]].pos) - vec3(_vertices[_indices[i]].pos),
					vec3(_vertices[_indices[i + 2]].pos) - vec3(_vertices[_indices[i]].pos)));

				_vertices[_indices[i + 0]].normal += normal;
				_vertices[_indices[i + 1]].normal += normal;
				_vertices[_indices[i + 2]].normal += normal;			
			}

			for(auto& vertex : _vertices)
			{
				vertex.normal = atta::normalize(vertex.normal);
			}
		}

		//---------- Finished parsing ----------//
		eval.stop();
		Log::info("Mesh", "Finished loading [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}

	void Mesh::generateBoxMesh()
	{
		LocalEvaluator eval;
		//---------- Calculate vertices ----------//

		std::vector<vec3> vertices = {	vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5), 
										vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), 
										vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5), 
										vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5)};
		std::vector<vec3> normals = {	vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0),
										vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0),
										vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0)};
		// TODO texture

		Vertex vertex = {
			.pos = vertices[0],
			.normal = normals[0],
			.texCoord = vec2(0,0),
			.materialIndex = 0
		};

		// Top
		for(int i=0; i<4; i++)
		{
			vertex.pos = vertices[i];
			vertex.normal = normals[0];
			_vertices.push_back(vertex);
		}
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);

		// Bottom
		for(int i=0; i<4; i++)
		{
			vertex.pos = vertices[4+i];
			vertex.normal = normals[1];
			_vertices.push_back(vertex);
		}
		_indices.push_back(4);
		_indices.push_back(7);
		_indices.push_back(6);
		_indices.push_back(4);
		_indices.push_back(6);
		_indices.push_back(5);
		
		//---------- Finished generating ----------//
		eval.stop();
		Log::info("Mesh", "Finished generating [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}
}
