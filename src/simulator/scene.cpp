//--------------------------------------------------
// Robot Simulator
// scene.cpp
// Date: 16/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "scene.h"
#include <memory>
#include <cstring>
#include "vulkan/vertex.h"
#include "vulkan/material.h"
#include "vulkan/buffer.h"
#include "vulkan/device.h"
#include "vulkan/stagingBuffer.h"
#include "physics/constraints/fixedConstraint.h"
#include "helpers/drawHelper.h"
#include "objects/basic/box.h"
#include "objects/basic/cylinder.h"
#include "objects/basic/sphere.h"

Scene::Scene():
	_maxLineCount(9999), _maxRTInstanceCount(1000)
{
	_device = nullptr;
	_physicsEngine = new PhysicsEngine();
	_commandPool = nullptr;

	// Load basic models to the memory
	_models.push_back(new Model("plane"));
	_models.push_back(new Model("box"));
	_models.push_back(new Model("sphere"));
	_models.push_back(new Model("cylinder"));
}

Scene::~Scene()
{
	if(_physicsEngine != nullptr)
	{
		delete _physicsEngine;
		_physicsEngine = nullptr;
	}

	if(_vertexBuffer != nullptr)
	{
		delete _vertexBuffer;
		_vertexBuffer = nullptr;
	}

	if(_indexBuffer != nullptr)
	{
		delete _indexBuffer;
		_indexBuffer = nullptr;
	}

	if(_materialBuffer != nullptr)
	{
		delete _materialBuffer;
		_materialBuffer = nullptr;
	}

	if(_offsetBuffer != nullptr)
	{
		delete _offsetBuffer;
		_offsetBuffer = nullptr;
	}

	if(_instanceBuffer != nullptr)
	{
		delete _instanceBuffer;
		_instanceBuffer = nullptr;
	}

	if(_aabbBuffer != nullptr)
	{
		delete _aabbBuffer;
		_aabbBuffer = nullptr;
	}
	
	if(_proceduralBuffer != nullptr)
	{
		delete _proceduralBuffer;
		_proceduralBuffer = nullptr;
	}

	if(_lineVertexBuffer != nullptr)
	{
		delete _lineVertexBuffer;
		_lineVertexBuffer = nullptr;
	}

	if(_lineIndexBuffer != nullptr)
	{
		delete _lineIndexBuffer;
		_lineIndexBuffer = nullptr;
	}

	for(auto& model : _models)
	{
		if(model != nullptr)
		{
			delete model;
			model = nullptr;
		}
	}

	for(auto& object : _objects)
	{
		if(object != nullptr)
		{
			delete object;
			object = nullptr;
		}
	}

	// TODO delete when texture isnt deleted by the model
	for(auto& texture : _textures)
	{
		if(texture != nullptr)
		{
			delete texture;
			texture = nullptr;
		}
	}
}

void Scene::loadObject(std::string fileName)
{
	// Load model to memory
	_models.push_back(new Model(fileName));
}

void Scene::addObject(Object* object)
{
	_objects.push_back(object);
	_physicsEngine->addObjectPhysics(_objects.back()->getObjectPhysics());
}

void Scene::addComplexObject(Object* object)
{
	_objects.push_back(object);
	_physicsEngine->addObjectPhysics(_objects.back()->getObjectPhysics());
	for(auto child : object->getChildren())
	{
		addComplexObject(child);
	}
}

void Scene::createBuffers(CommandPool* commandPool)
{
	_commandPool = commandPool;
	_device = commandPool->getDevice();
	_proceduralBuffer = nullptr;

	_textures.push_back(new Texture(_device, _commandPool, "assets/models/cube_multi/cube_multi.png"));

	// Concatenate all the models
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;
	std::vector<glm::uvec2> offsets;
	std::vector<std::pair<glm::vec3, glm::vec3>> aabbs;
	std::vector<glm::vec4> procedurals;

	for(const auto model : _models)
	{
		// Remember the index, vertex offsets.
		const uint32_t indexOffset = static_cast<uint32_t>(indices.size());
		const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
		const auto materialOffset = static_cast<uint32_t>(materials.size());

		offsets.emplace_back(indexOffset, vertexOffset);

		// Copy model data one after the other.
		vertices.insert(vertices.end(), model->getVertices().begin(), model->getVertices().end());
		indices.insert(indices.end(), model->getIndices().begin(), model->getIndices().end());
		materials.insert(materials.end(), model->getMaterials().begin(), model->getMaterials().end());

		// Adjust the material id.
		for(size_t i = vertexOffset; i != vertices.size(); i++)
		{
			vertices[i].materialIndex += materialOffset;
		}

		// Add optional procedurals
		//const auto sphere = dynamic_cast<const Sphere*>(model->getProcedural());
		//if (sphere != nullptr)
		//{
		//	aabbs.push_back(sphere->boundingBox());
		//	procedurals.emplace_back(sphere->center, sphere->radius);
		//}
		//else
		//{
			aabbs.emplace_back();
			procedurals.emplace_back();
		//}
	}

	// Ray tracing flasg
	const auto flag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

	// Generate simulation grid (line buffer)
	genGridLines();
	_lineIndexCount = _hostLineIndex.size();
	_indexGridCount = _lineIndexCount;
	// TODO limit 1000 objects (ray tracing)
	std::vector<InstanceInfo> instances(_maxRTInstanceCount);

	createSceneBuffer(_vertexBuffer, 		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT|flag, vertices);
	createSceneBuffer(_indexBuffer, 		VK_BUFFER_USAGE_INDEX_BUFFER_BIT|flag, 	indices);
	createSceneBuffer(_materialBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,		materials);
	createSceneBuffer(_offsetBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	offsets);
	createSceneBuffer(_aabbBuffer, 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	aabbs);
	createSceneBuffer(_proceduralBuffer, 	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	procedurals);
	createSceneBuffer(_lineVertexBuffer, 	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 	_hostLineVertex, _maxLineCount*2);
	createSceneBuffer(_lineIndexBuffer, 	VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 	_hostLineIndex, _maxLineCount*2);
	createSceneBuffer(_instanceBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	instances);
}

void Scene::updateRayTracingBuffers()
{
	std::vector<InstanceInfo> instances;
	for(auto object : _objects)
	{
		InstanceInfo instanceInfo;
		instanceInfo.transform = object->getModelMat();
		instanceInfo.transformIT = glm::transpose(glm::inverse(object->getModelMat()));
		// Set object color
		instanceInfo.diffuse = glm::vec4(-1,-1,-1,1);
		if(object->getType() == "Box")
			instanceInfo.diffuse = glm::vec4(((Box*)object)->getColor(),1);
		if(object->getType() == "Cylinder")
			instanceInfo.diffuse = glm::vec4(((Cylinder*)object)->getColor(),1);
		if(object->getType() == "Sphere")
			instanceInfo.diffuse = glm::vec4(((Sphere*)object)->getColor(),1);

		instances.push_back(instanceInfo);
	}

	// Update buffer
	if(_commandPool==nullptr)
		return;
	//printf("Command pool OK\n");
	Device* device = _commandPool->getDevice();
	StagingBuffer* stagingBuffer = new StagingBuffer(device, instances.data(), sizeof(InstanceInfo)*instances.size());
	_instanceBuffer->copyFrom(_commandPool, stagingBuffer->handle(), sizeof(InstanceInfo)*instances.size());
	delete stagingBuffer;
	stagingBuffer = nullptr;
}

void Scene::updatePhysics(float dt)
{
	_physicsEngine->stepPhysics(dt);
}


template <class T>
void Scene::createSceneBuffer(Buffer*& buffer,
		const VkBufferUsageFlags usage, 
		const std::vector<T>& content,
		const uint32_t maxElements)
{

	int size;
	if(maxElements == 0)
		size = sizeof(content[0]) * content.size();
	else
		size = sizeof(content[0]) * maxElements;

	buffer = new Buffer(_commandPool->getDevice(), 
			size, 
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	copyFromStagingBuffer(buffer, content);
}

template <class T>
void Scene::copyFromStagingBuffer(Buffer* dstBuffer, const std::vector<T>& content)
{
	Device* device = _commandPool->getDevice();
	const auto contentSize = sizeof(content[0]) * content.size();

	// Create a temporary host-visible staging buffer.
	Buffer* stagingBuffer = new Buffer(
			device, 
			contentSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


	// Copy the host data into the staging buffer.
	void* data = stagingBuffer->mapMemory(0, contentSize);
	std::memcpy(data, content.data(), contentSize);
	stagingBuffer->unmapMemory();

	// Copy the staging buffer to the device buffer.
	dstBuffer->copyFrom(_commandPool, stagingBuffer->handle(), contentSize);

	// Delete the buffer before the memory
	delete stagingBuffer;
	stagingBuffer = nullptr;
}

void Scene::addLine(glm::vec3 p0, glm::vec3 p1, glm::vec3 color)
{
	if((_hostLineIndex.size()/2+1)>_maxLineCount)
		return;

	const glm::vec2 texCoord(0,0);

	Vertex v1 = {p0, color, texCoord, 0};
	Vertex v2 = {p1, color, texCoord, 0};
	_hostLineVertex.push_back(v1);
	_hostLineVertex.push_back(v2);
	_hostLineIndex.push_back(_hostLineIndex.size());
	_hostLineIndex.push_back(_hostLineIndex.size());
	_lineIndexCount = _hostLineIndex.size();
}

void Scene::cleanLines()
{
	int currSize = _hostLineVertex.size();
	while(_hostLineVertex.size()>_indexGridCount)
	{
		_hostLineVertex.pop_back();
		_hostLineIndex.pop_back();
	}

	_lineIndexCount = _hostLineIndex.size();
}

void Scene::drawCollisionShapes()
{
	//for(auto object : _objects)
	//{
	//	btCollisionShape* collision = object->getObjectPhysics()->getCollisionShape();
	//	switch(collision->getShapeType())
	//	{
	//		case BOX_SHAPE_PROXYTYPE:
	//			const btBoxShape* box = static_cast<const btBoxShape*>(collision);
	//			// get the 'halfSize' of the box
	//			glm::vec3 size = PhysicsEngine::bt2glm(box->getHalfExtentsWithMargin())*2.0f;
	//			//printf("Draw box (%f, %f, %f)\n", halfSize.x, halfSize.y, halfSize.z);
	//			//std::vector<std::pair<glm::vec3, glm::vec3>> boxLines = DrawHelper::getBoxLines()

	//			break;
	//	}
	//}
}

void Scene::updateLineBuffer()
{
	if(_device == nullptr || _commandPool==nullptr)
		return;

	StagingBuffer* stagingBufferVertex = new StagingBuffer(_device, _hostLineVertex.data(), sizeof(Vertex)*_maxLineCount*2);
	StagingBuffer* stagingBufferIndex = new StagingBuffer(_device, _hostLineIndex.data(), sizeof(uint32_t)*_maxLineCount*2);

	_lineVertexBuffer->copyFrom(_commandPool, stagingBufferVertex->handle(), sizeof(Vertex)*_maxLineCount*2);
	_lineIndexBuffer->copyFrom(_commandPool, stagingBufferIndex->handle(), sizeof(uint32_t)*_maxLineCount*2);

	delete stagingBufferVertex;
	delete stagingBufferIndex;
	stagingBufferVertex = nullptr;
	stagingBufferIndex = nullptr;

	_lineIndexCount = _hostLineIndex.size();
}

void Scene::genGridLines()
{
	int gridSize = 100;
	const glm::vec3 gridColor = {0.35, 0.35, 0.35};
	const glm::vec2 texCoord(0,0);

	// X lines
	for(int i=-gridSize;i<gridSize;i++)
	{
		const glm::vec3 pos1(
			-gridSize,
			0,
			i);

		const glm::vec3 pos2(
			gridSize,
			0,
			i);
		
		addLine(pos1, pos2, gridColor);
	}

	// Z lines
	for(int i=-gridSize;i<gridSize;i++)
	{
		const glm::vec3 pos1(
			i,
			0,
			-gridSize);

		const glm::vec3 pos2(
			i,
			0,
			gridSize);

		addLine(pos1, pos2, gridColor);
	}
}

void Scene::linkObjects()
{
	//for(auto object : _objects)
	//{
	//	Constraint* constraint = object->getParentConstraint();
	//	if(constraint != nullptr)
	//	{
	//		ObjectPhysics* childPhysics = object->getObjectPhysics();
	//		ObjectPhysics* parentPhysics = object->getParent()->getObjectPhysics();
	//		_physicsEngine->getWorld()->addConstraint(constraint->createConstraint(childPhysics, parentPhysics), true);
	//	}
	//}
}
