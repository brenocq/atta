//--------------------------------------------------
// Robot Simulator
// model.cpp
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "model.h"
#include "simulator/helpers/log.h"
#include <chrono>
#include <glm/gtc/matrix_inverse.hpp>

int Model::textureId = 0;
int Model::qtyModels = 0;
std::vector<std::string> Model::currentModels = {};
std::vector<uint32_t> Model::vertexOffsets = {0};
std::vector<uint32_t> Model::indexOffsets = {0};
std::vector<uint32_t> Model::verticesSize = {};
std::vector<uint32_t> Model::indicesSize = {};

Model::Model(std::string fileName):
	_fileName(fileName), _modelIndex(-1), _procedural(nullptr)
{
	// Check if model was previously loaded
	for(int i=0; i<(int)Model::currentModels.size(); i++)
	{
		if(Model::currentModels[i] == _fileName)
		{
			_modelIndex = i;
			break;
		}
	}

	//---------- Load model if it is the first time it is used ----------//
	if(_modelIndex == -1)
	{
		if(fileName.find("atta::")!=std::string::npos)
		{
			//----- Atta custom model -----//
			// Check sphere
			if(fileName.find("sphere")!=std::string::npos)
			{
				createSphereModel();
			}
			else if(fileName.find("cylinder")!=std::string::npos)
			{
				createCylinderModel();
			}
			else if(fileName.find("capsule")!=std::string::npos)
			{
				createCapsuleModel();
			}
		}
		else
		{
			//----- Load model from file -----//
				std::cout << std::endl << BOLDGREEN << "[Model]" << RESET << GREEN << " Loading model " << WHITE << fileName << RESET << std::endl;
				loadModel();
		}

		// Save model data
		_modelIndex = Model::qtyModels++;
		Model::currentModels.push_back(_fileName);

		uint32_t lastV = Model::vertexOffsets.size() > 0 ? Model::vertexOffsets.back() : 0;
		uint32_t lastI = Model::indexOffsets.size() > 0 ? Model::indexOffsets.back() : 0;

		Model::vertexOffsets.push_back(lastV + _vertices.size());
		Model::indexOffsets.push_back(lastI + _indices.size());
		Model::verticesSize.push_back(_vertices.size());
		Model::indicesSize.push_back(_indices.size());
	}

}

Model::Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural):
	_procedural(procedural),
	_vertices(std::move(vertices)), 
	_indices(std::move(indices)),
	_materials(std::move(materials))
{
}

Model::~Model()
{
}

void Model::loadModel()
{
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	std::string modelPath = "assets/models/"+_fileName+"/"+_fileName+".obj";

	std::cout << GREEN << "\tLoading Vertices... " << WHITE;
	begin = std::chrono::steady_clock::now();

	tinyobj::ObjReader objReader;
	
	if (!objReader.ParseFromFile(modelPath))
	{
		std::cout << std::endl << BOLDRED << "[Model]" << RED << " Failed to load model " + modelPath + ": " + objReader.Error()<< RESET << std::endl;
		exit(1);
	}

	if (!objReader.Warning().empty())
	{
		std::cout << std::endl << BOLDYELLOW << "[Model]" << YELLOW << objReader.Warning() << RESET << std::endl << std::flush;
	}

	//---------- Materials ----------//
	for(const auto& material : objReader.GetMaterials())
	{
		Material m = {};

		m.diffuse = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
		m.diffuseTextureId = -1;
		//Material m = Material::diffuseLight(glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]), -1);

		_materials.emplace_back(m);
	}

	if(_materials.empty())
	{
		//Material m{};

		//m.diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0);
		//m.diffuseTextureId = -1;
		//Material m = Material::dielectric(1.5f);
		Material m = Material::diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), -1);

		_materials.emplace_back(m);
	}

	//---------- Geometry ----------//
	const auto& objAttrib = objReader.GetAttrib();

	std::unordered_map<Vertex, uint32_t> uniqueVertices(objAttrib.vertices.size());
	size_t faceId = 0;

	for (const auto& shape : objReader.GetShapes())
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
	// Using flat normals would mean creating more vertices than we currently have, so for simplicity and better visuals we don't do it.
	// See https://stackoverflow.com/questions/12139840/obj-file-averaging-normals.
	if (objAttrib.normals.empty())
	{
		std::vector<glm::vec3> normals(_vertices.size());
		
		for (size_t i = 0; i < _indices.size(); i += 3)
		{
			const auto normal = glm::normalize(glm::cross(
				glm::vec3(_vertices[_indices[i + 1]].pos) - glm::vec3(_vertices[_indices[i]].pos),
				glm::vec3(_vertices[_indices[i + 2]].pos) - glm::vec3(_vertices[_indices[i]].pos)));

			_vertices[_indices[i + 0]].normal += normal;
			_vertices[_indices[i + 1]].normal += normal;
			_vertices[_indices[i + 2]].normal += normal;			
		}

		for(auto& vertex : _vertices)
		{
			vertex.normal = glm::normalize(vertex.normal);
		}
	}

	end = std::chrono::steady_clock::now();

	std::cout << WHITE
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
		<< "ms" 
		<< " (" << _vertices.size() << " vertices, " << _indices.size() << " indices)"	
		<< RESET << std::endl;
}

void Model::transform(const glm::mat4& transform)
{
	// Can be used to scale/rotate/translate the model before loading the vertices to the memory
	const auto transformIT = glm::inverseTranspose(transform);

	for (auto& vertex : _vertices)
	{
		vertex.pos = transform * glm::vec4(vertex.pos, 1);
		vertex.normal = transformIT * glm::vec4(vertex.normal, 0);
	}
}

void Model::createSphereModel()
{
	int qtyLong = 32;
	int qtyLat = 16;
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
			vertex.normal = glm::normalize(vertex.pos);
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

	//---------- Create materials ----------//
	Material m = Material::diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), -1);
	_materials.emplace_back(m);

	Log::success("Model", "Sphere mesh created");
}

void Model::createCylinderModel()
{
	int qtySections = 32;
	float radius = 0.5;
	float height = 1;
	//---------- Create vertices ----------//
	// Vertices with normal tangent to circle
	for(int t=0; t<qtySections; t++)
	{
		float theta = t*(2*M_PI/qtySections);

		Vertex vertex;
		vertex.normal = glm::normalize(glm::vec3(cos(theta), 0, sin(theta)));
		vertex.materialIndex = 0;

		// Bottom
		vertex.pos = {radius*cos(theta), -height/2, radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),0};
		_vertices.push_back(vertex);

		// Top
		vertex.pos = {radius*cos(theta), height/2, radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),1};
		_vertices.push_back(vertex);
	}
	// Top
	for(int t=0; t<qtySections; t++)
	{
		float theta = t*(2*M_PI/qtySections);

		Vertex vertex;
		vertex.normal = glm::vec3(0, 1, 0);
		vertex.materialIndex = 0;

		vertex.pos = {radius*cos(theta), height/2, radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),1};
		_vertices.push_back(vertex);
	}
	// Bottom
	for(int t=0; t<qtySections; t++)
	{
		float theta = t*(2*M_PI/qtySections);

		Vertex vertex;
		vertex.normal = glm::vec3(0, -1, 0);
		vertex.materialIndex = 0;

		vertex.pos = {radius*cos(theta), -height/2, radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),1};
		_vertices.push_back(vertex);
	}
	// Top center
	{
		Vertex vertex;
		vertex.normal = glm::vec3(0, 1, 0);
		vertex.materialIndex = 0;
		vertex.pos = {0, height/2, 0};
		vertex.texCoord = {0,1};
		_vertices.push_back(vertex);
	}

	// Bottom center
	{
		Vertex vertex;
		vertex.normal = glm::vec3(0, -1, 0);
		vertex.materialIndex = 0;
		vertex.pos = {0, -height/2, 0};
		vertex.texCoord = {0,1};
		_vertices.push_back(vertex);
	}

	//---------- Create indices ----------//
	// Side face indices
	for(int i=0; i<qtySections*2-2; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(i+2);
	}
	_indices.push_back(qtySections*2-1);
	_indices.push_back(qtySections*2-2);
	_indices.push_back(2);

	_indices.push_back(0);
	_indices.push_back(1);
	_indices.push_back(qtySections*2-1);
	// Top face indices
	for(int i=qtySections*2; i<qtySections*3-1; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(qtySections*4);
	}
	_indices.push_back(qtySections*2);
	_indices.push_back(qtySections*3-1);
	_indices.push_back(qtySections*4);
	// Bottom face indices
	for(int i=qtySections*3; i<qtySections*4-1; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(qtySections*4+1);
	}
	_indices.push_back(qtySections*3);
	_indices.push_back(qtySections*4-1);
	_indices.push_back(qtySections*4+1);

	//---------- Create materials ----------//
	Material m = Material::diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), -1);
	_materials.emplace_back(m);

	Log::success("Model", "Cylinder mesh created");
}

void Model::createCapsuleModel()
{
	int qtyLong = 32;
	int qtyLat = 16;
	float radius = 0.5;
	float height = 2.0;
	//---------- Create vertices ----------//
	// Top sphere
	for(int p=1; p<qtyLat/2+1; p++)
	{
		for(int t=0; t<qtyLong; t++)
		{
			float phi = M_PI-p*(M_PI/qtyLat);
			float theta = t*(2*M_PI/qtyLong);

			Vertex vertex;
			vertex.pos = {radius*cos(theta)*sin(phi), -radius*cos(phi) + height/2-radius, radius*sin(theta)*sin(phi)};
			vertex.normal = glm::normalize(glm::vec3(radius*cos(theta)*sin(phi), -radius*cos(phi), radius*sin(theta)*sin(phi)));
			vertex.materialIndex = 0;
			vertex.texCoord = {theta/2*M_PI,(phi+M_PI/2)/M_PI};
			_vertices.push_back(vertex);
		}
	}
	// Bottom sphere
	for(int p=1; p<qtyLat/2+1; p++)
	{
		for(int t=0; t<qtyLong; t++)
		{
			float phi = M_PI-p*(M_PI/qtyLat);
			float theta = t*(2*M_PI/qtyLong);

			Vertex vertex;
			vertex.pos = {radius*cos(theta)*sin(phi), radius*cos(phi) - (height/2-radius), radius*sin(theta)*sin(phi)};
			vertex.normal = glm::normalize(glm::vec3(radius*cos(theta)*sin(phi), radius*cos(phi), radius*sin(theta)*sin(phi)));
			vertex.materialIndex = 0;
			vertex.texCoord = {theta/2*M_PI,(phi+M_PI/2)/M_PI};
			_vertices.push_back(vertex);
		}
	}

	int indexCircle = _vertices.size();
	// Circle
	for(int t=0; t<qtyLong; t++)
	{
		float theta = t*(2*M_PI/qtyLong);

		Vertex vertex;
		vertex.normal = glm::normalize(glm::vec3(cos(theta), 0, sin(theta)));
		vertex.materialIndex = 0;

		// Bottom
		vertex.pos = {radius*cos(theta), -(height/2-radius), radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),0};
		_vertices.push_back(vertex);

		// Top
		vertex.pos = {radius*cos(theta), (height/2-radius), radius*sin(theta)};
		vertex.texCoord = {theta/(2*M_PI),1};
		_vertices.push_back(vertex);
	}
	// Extreme vertices
	{
		Vertex vertex;
		vertex.materialIndex = 0;
		vertex.texCoord = {0,0};

		// Top
		vertex.pos = {0, height/2, 0};
		vertex.normal = glm::vec3(0, 1, 0);
		_vertices.push_back(vertex);

		// Bottom
		vertex.pos = {0, -height/2, 0};
		vertex.normal = glm::vec3(0, -1, 0);
		_vertices.push_back(vertex);
	}

	//---------- Create indices ----------//
	// Top sphere
	for(int j=0; j<qtyLat/2-1; j++)
	{
		for(int i=0; i<qtyLong; i++)
		{
			_indices.push_back(j*qtyLong+i);
			_indices.push_back(j*qtyLong+i+1);
			_indices.push_back((j+1)*qtyLong+i);

			if(i+1<qtyLong)
			{
				_indices.push_back((j+1)*qtyLong+i);
				_indices.push_back((j+1)*qtyLong+i+1);
				_indices.push_back(j*qtyLong+i+1);
			}
			else
			{
				_indices.push_back(j*qtyLong);
				_indices.push_back((j+1)*qtyLong-1);
				_indices.push_back((j+1)*qtyLong);
			}
		}
	}

	// Bottom sphere
	for(int j=qtyLat/2; j<qtyLat-1; j++)
	{
		for(int i=0; i<qtyLong; i++)
		{
			_indices.push_back(j*qtyLong+i);
			_indices.push_back(j*qtyLong+i+1);
			_indices.push_back((j+1)*qtyLong+i);

			if(i+1<qtyLong)
			{
				_indices.push_back((j+1)*qtyLong+i);
				_indices.push_back((j+1)*qtyLong+i+1);
				_indices.push_back(j*qtyLong+i+1);
			}
			else
			{
				_indices.push_back(j*qtyLong);
				_indices.push_back((j+1)*qtyLong-1);
				_indices.push_back((j+1)*qtyLong);
			}
		}
	}
	
	// Side face indices
	for(int i=indexCircle; i<indexCircle+qtyLong*2-2; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(i+2);
	}
	_indices.push_back(indexCircle+qtyLong*2-1);
	_indices.push_back(indexCircle+qtyLong*2-2);
	_indices.push_back(indexCircle+2);

	_indices.push_back(indexCircle+0);
	_indices.push_back(indexCircle+1);
	_indices.push_back(indexCircle+qtyLong*2-1);

	// Extreme triangles top
	for(int i=0;i<qtyLong-1; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(_vertices.size()-2);
	}
	_indices.push_back(0);
	_indices.push_back(qtyLong-1);
	_indices.push_back(_vertices.size()-2);

	// Extreme triangles bottom
	int startExB = qtyLat/2*qtyLong;
	for(int i=startExB;i<startExB+qtyLong-1; i++)
	{
		_indices.push_back(i);
		_indices.push_back(i+1);
		_indices.push_back(_vertices.size()-1);
	}
	_indices.push_back(startExB);
	_indices.push_back(startExB+qtyLong-1);
	_indices.push_back(_vertices.size()-1);

	//---------- Create materials ----------//
	Material m = Material::diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), -1);
	_materials.emplace_back(m);

	Log::success("Model", "Sphere mesh created");
}
