//--------------------------------------------------
// Robot Simulator
// scene.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <memory>
#include "defines.h"
#include "physics/physicsEngine.h"
#include "vulkan/model.h"
#include "vulkan/texture.h"
#include "vulkan/buffer.h"
#include "vulkan/commandPool.h"
#include "vulkan/material.h"
#include "vulkan/helpers.h"
#include "object.h"

using namespace atta::phy;

class Scene
{
	public:
		Scene();
		~Scene();

		void loadObject(std::string fileName);
		void addObject(Object* object);
		void createBuffers(std::shared_ptr<CommandPool> commandPool);

		void linkObjects();
		void updatePhysics(float dt);

		//--------- Getters ----------//
		PhysicsEngine* getPhysicsEngine() const { return _physicsEngine; }
		//----- Simulation specific ------//
		std::vector<Object*> getObjects() const { return _objects; };
		std::vector<Model*> getModels() const { return _models; };
		std::vector<Texture*> getTextures() const { return _textures; };

		Buffer* getVertexBuffer() const { return _vertexBuffer; }
		Buffer* getIndexBuffer() const { return _indexBuffer; }
		Buffer* getMaterialBuffer() const { return _materialBuffer; }
		Buffer* getOffsetBuffer() const { return _offsetBuffer; }
		Buffer* getInstanceBuffer() const { return _instanceBuffer; }
		Buffer* getAabbBuffer() const { return _aabbBuffer; }
		Buffer* getProceduralBuffer() const { return _proceduralBuffer; }
		bool hasProcedurals() const { return static_cast<bool>(_proceduralBuffer); }
		Texture* getEnvTexture() const { return _envTexture; }
		Texture* getEnvIrrTexture() const { return _envIrrTexture; }

		//----- Line debugger -----//
		void addLine(glm::vec3 p0, glm::vec3 p1, glm::vec3 color);
		void cleanLines();
		void drawCollisionShapes();
		void updateLineBuffer();

		//----- Simulator specific -----//
		Buffer* getLineVertexBuffer() const { return _lineVertexBuffer; }
		Buffer* getLineIndexBuffer() const { return _lineIndexBuffer; }
		uint32_t getLineIndexCount() const {return _lineIndexCount; }
		Buffer* getSkyboxVertexBuffer() const { return _skyboxVertexBuffer; }
		Buffer* getSkyboxIndexBuffer() const { return _skyboxIndexBuffer; }
		uint32_t getSkyboxIndexCount() const {return _skyboxIndexCount; }

		//--- Ray tracing ---//
		void updateRayTracingBuffers();

	private:
		template <class T>
		void createSceneBuffer(Buffer*& buffer,
			const VkBufferUsageFlags usage, 
			const std::vector<T>& content,
			const uint32_t maxElements=0);

		template <class T>
		void copyFromStagingBuffer(Buffer* dstBuffer, const std::vector<T>& content);

		void genGridLines();

		// Objects in the scene
		std::vector<Object*> _objects;
		// Models and textures loaded to the memory
		std::vector<Model*> _models;
		std::vector<Texture*> _textures;

		std::shared_ptr<Device> _device;
		std::shared_ptr<CommandPool> _commandPool;
		Buffer* _vertexBuffer;
		Buffer* _indexBuffer;
		Buffer* _materialBuffer;
		Buffer* _offsetBuffer;
		Buffer* _instanceBuffer;
		Buffer* _aabbBuffer;
		Buffer* _proceduralBuffer;

		// Simulator specific
		uint32_t _maxLineCount;// Maximum number of lines that can be store in memory
		uint32_t _maxRTInstanceCount;// Maximum number of lines that can be store in memory
		uint32_t _lineIndexCount;// Current index count (line count*2)
		uint32_t _indexGridCount;// Number of lines in the grid
		Buffer* _lineVertexBuffer;
		Buffer* _lineIndexBuffer;
		std::vector<Vertex> _hostLineVertex;
		std::vector<uint32_t> _hostLineIndex;

		Buffer* _skyboxVertexBuffer;
		Buffer* _skyboxIndexBuffer;
		uint32_t _skyboxIndexCount;

		//---------- Lighting ----------//
		Texture* _envTexture;// Environment image reference (high resolution)
		Texture* _envIrrTexture;// Environment irradiance

		//---------- Physics ----------//
		PhysicsEngine* _physicsEngine;
};

#endif// SCENE_H
