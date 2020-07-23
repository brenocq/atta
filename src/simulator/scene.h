//--------------------------------------------------
// Robot Simulator
// scene.h
// Date: 16/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include "defines.h"
#include "vulkan/model.h"
#include "vulkan/texture.h"
#include "vulkan/buffer.h"
#include "vulkan/commandPool.h"

class Scene
{
	public:
	Scene(CommandPool* commandPool, std::vector<Model*> models, std::vector<Texture*> textures, bool enableRayTracing=false);
	~Scene();

	void addModel(Model* model);
	std::vector<Model*> getModels() const { return _models; };
	std::vector<Texture*> getTextures() const { return _textures; };
	Buffer* getVertexBuffer() const { return _vertexBuffer; }
	Buffer* getIndexBuffer() const { return _indexBuffer; }

	private:
	template <class T>
	void createSceneBuffer(Buffer*& buffer,
		const VkBufferUsageFlags usage, 
		const std::vector<T>& content);

	std::vector<Model*> _models;
	std::vector<Texture*> _textures;

	CommandPool* _commandPool;
	Buffer* _vertexBuffer;
	Buffer* _indexBuffer;
	//Buffer* _materialBuffer;
	Buffer* _offsetBuffer;
};

#endif// SCENE_H
