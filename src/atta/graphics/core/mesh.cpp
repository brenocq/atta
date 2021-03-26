//--------------------------------------------------
// Atta Graphics
// mesh.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <unordered_map>
#include <atta/graphics/core/mesh.h>
#include <atta/extern/tinyObjLoader.h>
#include <atta/helpers/evaluator.h>
#include <atta/helpers/log.h>

namespace atta
{
	std::vector<Mesh*> Mesh::allMeshes = {};

	Mesh::Mesh(std::string meshName):
		_meshName(meshName)
	{
		_index = allMeshes.size();
		allMeshes.push_back(this);
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
			else if(meshName == "atta::cylinder")
			{
				generateCylinderMesh();
			}
			else if(meshName == "atta::plane")
			{
				generatePlaneMesh();
			}
			else if(meshName == "atta::sphere")
			{
				generateSphereMesh();
			}
		}
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::loadMesh()
	{
		LocalEvaluator eval;
		std::string objPath = "assets/models/"+_meshName;

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
			std::vector<vec3> normals(_vertices.size());
			
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

		// Top (0-3)
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

		// Bottom (4-7)
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

		// Right (8-11)
		vertex.normal = normals[2];
		vertex.pos = vertices[0];
		_vertices.push_back(vertex);
		vertex.pos = vertices[3];
		_vertices.push_back(vertex);
		vertex.pos = vertices[7];
		_vertices.push_back(vertex);
		vertex.pos = vertices[4];
		_vertices.push_back(vertex);

		_indices.push_back(8);
		_indices.push_back(9);
		_indices.push_back(10);
		_indices.push_back(10);
		_indices.push_back(11);
		_indices.push_back(8);

		// Left (12-15)
		vertex.normal = normals[3];
		vertex.pos = vertices[1];
		_vertices.push_back(vertex);
		vertex.pos = vertices[5];
		_vertices.push_back(vertex);
		vertex.pos = vertices[6];
		_vertices.push_back(vertex);
		vertex.pos = vertices[2];
		_vertices.push_back(vertex);

		_indices.push_back(12);
		_indices.push_back(13);
		_indices.push_back(14);
		_indices.push_back(14);
		_indices.push_back(15);
		_indices.push_back(12);

		// Front (16-19)
		vertex.normal = normals[4];
		vertex.pos = vertices[1];
		_vertices.push_back(vertex);
		vertex.pos = vertices[0];
		_vertices.push_back(vertex);
		vertex.pos = vertices[4];
		_vertices.push_back(vertex);
		vertex.pos = vertices[5];
		_vertices.push_back(vertex);

		_indices.push_back(16);
		_indices.push_back(17);
		_indices.push_back(18);
		_indices.push_back(18);
		_indices.push_back(19);
		_indices.push_back(16);

		// Back (20-23)
		vertex.normal = normals[5];
		vertex.pos = vertices[3];
		_vertices.push_back(vertex);
		vertex.pos = vertices[2];
		_vertices.push_back(vertex);
		vertex.pos = vertices[6];
		_vertices.push_back(vertex);
		vertex.pos = vertices[7];
		_vertices.push_back(vertex);

		_indices.push_back(20);
		_indices.push_back(21);
		_indices.push_back(22);
		_indices.push_back(22);
		_indices.push_back(23);
		_indices.push_back(20);
		
		//---------- Finished generating ----------//
		eval.stop();
		Log::info("Mesh", "Finished generating [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}

	void Mesh::generateCylinderMesh()
	{
		const int circleFaces = 32;
		LocalEvaluator eval;
		//---------- Calculate vertices ----------//

		// TODO texture
		Vertex vertex = {
			.pos = vec3(0,.5,0),
			.normal = vec3(0,1,0),
			.texCoord = vec2(0,0),
			.materialIndex = 0
		};

		// Create top disk
		_vertices.push_back(vertex);
		for(int i=0; i<circleFaces; i++)
		{
			float theta = (i/float(circleFaces))*2*M_PI;
			vertex.pos = vec3(cos(theta), 0.5, sin(theta));
			_vertices.push_back(vertex);

			_indices.push_back(0);
			_indices.push_back((i+1)%circleFaces+1);
			_indices.push_back(i+1);
		}

		// Create bottom disk
		
		// Create faces

		// TODO finish shape creating
		eval.stop();
		Log::info("Mesh", "Finished generating [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}

	void Mesh::generatePlaneMesh()
	{
		LocalEvaluator eval;

		std::vector<vec3> vertices = {	vec3(0.5, 0.0, 0.5), vec3(-0.5, 0.0, 0.5), 
										vec3(-0.5, 0.0, -0.5), vec3(0.5, 0.0, -0.5)};
		vec3 normal = vec3(0.0, 1.0, 0.0);

		Vertex vertex = {
			.pos = vertices[0],
			.normal = normal,
			.texCoord = vec2(0,0),
			.materialIndex = 0
		};

		for(int i=0; i<4; i++)
		{
			vertex.pos = vertices[i];
			vertex.texCoord = vec2(vertices[i].x > 0.0f, vertices[i].z > 0.0f);
			_vertices.push_back(vertex);
		}
		_indices.push_back(0);
		_indices.push_back(1);
		_indices.push_back(2);
		_indices.push_back(0);
		_indices.push_back(2);
		_indices.push_back(3);

		//---------- Finished generating ----------//
		eval.stop();
		Log::info("Mesh", "Finished generating [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}

	void Mesh::generateSphereMesh()
	{
		LocalEvaluator eval;

		int qtyLong = 32;
		int qtyLat = 32;
		float radius = 0.5;
		//---------- Create vertices ----------//
		for(int p=0; p<qtyLat+1; p++)
		{
			for(int t=0; t<qtyLong; t++)
			{
				float phi = -M_PI+p*(M_PI/qtyLat);
				float theta = t*(2*M_PI/qtyLong);

				Vertex vertex;
				vertex.pos = {radius*cos(theta)*sin(phi), radius*cos(phi), radius*sin(theta)*sin(phi)};
				vertex.normal = normalize(vertex.pos);
				vertex.materialIndex = 0;
				vertex.texCoord = {theta/2*M_PI,(phi+M_PI/2)/M_PI};
				_vertices.push_back(vertex);
			}
		}

		//---------- Create indices ----------//
		for(int j=0; j<qtyLat; j++)
		{
			for(int i=0; i<qtyLong; i++)
			{
				_indices.push_back(j*qtyLong+i);
				_indices.push_back(j*qtyLong+i+1);
				_indices.push_back((j+1)*qtyLong+i);

				_indices.push_back((j+1)*qtyLong+i);
				_indices.push_back((j+1)*qtyLong+i+1);
				_indices.push_back(j*qtyLong+i+1);
			}
		}

		//---------- Finished generating ----------//
		eval.stop();
		Log::info("Mesh", "Finished generating [b]$0[] - [w]$1ms ($2 vertices, $3 indices)", 
				_meshName, eval.getMs(), _vertices.size(), _indices.size());
	}
}
