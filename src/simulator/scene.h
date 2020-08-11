//--------------------------------------------------
// Robot Simulator
// scene.h
// Date: 16/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <memory>
#include "defines.h"
#include "vulkan/model.h"
#include "vulkan/texture.h"
#include "vulkan/buffer.h"
#include "vulkan/commandPool.h"
#include "vulkan/material.h"

class Scene
{
	public:
	Scene(CommandPool* commandPool, std::vector<Model*> models, std::vector<Texture*> textures, bool enableRayTracing=false);
	~Scene();

	//--------- Simulation specific ----------//
	void addModel(Model* model);
	std::vector<Model*> getModels() const { return _models; };
	std::vector<Texture*> getTextures() const { return _textures; };
	Buffer* getVertexBuffer() const { return _vertexBuffer; }
	Buffer* getIndexBuffer() const { return _indexBuffer; }
	Buffer* getMaterialBuffer() const { return _materialBuffer; }
	Buffer* getOffsetBuffer() const { return _offsetBuffer; }
	Buffer* getAabbBuffer() const { return _aabbBuffer; }
	Buffer* getProceduralBuffer() const { return _proceduralBuffer; }
	bool hasProcedurals() const { return static_cast<bool>(_proceduralBuffer); }

	//--------- Simulator specific ---------//
	Buffer* getLineVertexBuffer() const { return _lineVertexBuffer; }
	Buffer* getLineIndexBuffer() const { return _lineIndexBuffer; }
	uint32_t getLineIndexCount() const {return _lineIndexCount; }

	private:
	template <class T>
	void createSceneBuffer(Buffer*& buffer,
		const VkBufferUsageFlags usage, 
		const std::vector<T>& content);

	template <class T>
	void copyFromStagingBuffer(Buffer* dstBuffer, const std::vector<T>& content);

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> gridLines();

	std::vector<Model*> _models;
	std::vector<Texture*> _textures;

	CommandPool* _commandPool;
	Buffer* _vertexBuffer;
	Buffer* _indexBuffer;
	Buffer* _materialBuffer;
	Buffer* _offsetBuffer;
	Buffer* _aabbBuffer;
	Buffer* _proceduralBuffer;

	// Simulator specific
	uint32_t _lineIndexCount;
	Buffer* _lineVertexBuffer;
	Buffer* _lineIndexBuffer;
};

#endif// SCENE_H
