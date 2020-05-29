#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "model.h"
#include "texture.h"
#include "textureImage.h"
#include "../vulkan/commandPool.h"
#include "../vulkan/buffer.h"
#include "../vulkan/deviceMemory.h"

class Scene
{
public:
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator = (const Scene&) = delete;
	Scene& operator = (Scene&&) = delete;

	Scene(CommandPool* commandPool, std::vector<Model>&& models, std::vector<Texture>&& textures, bool usedForRayTracing);
	~Scene();

	const std::vector<Model>& models() const { return _models; }
	bool hasProcedurals() const { return static_cast<bool>(_proceduralBuffer); }

	Buffer* vertexBuffer() const { return _vertexBuffer; }
	Buffer* indexBuffer() const { return _indexBuffer; }
	Buffer* materialBuffer() const { return _materialBuffer; }
	Buffer* offsetsBuffer() const { return _offsetBuffer; }
	Buffer* aabbBuffer() const { return _aabbBuffer; }
	Buffer* proceduralBuffer() const { return _proceduralBuffer; }
	const std::vector<VkImageView> textureImageViews() const { return _textureImageViewHandles; }
	const std::vector<VkSampler> textureSamplers() const { return _textureSamplerHandles; }

private:
	template <class T>
	void copyFromStagingBuffer(CommandPool* commandPool, Buffer* dstBuffer, const std::vector<T>& content);

	template <class T>
	void createDeviceBuffer(CommandPool* commandPool, const VkBufferUsageFlags usage, const std::vector<T>& content, Buffer** buffer, DeviceMemory** memory);

	const std::vector<Model> _models;
	const std::vector<Texture> _textures;

	Buffer* _vertexBuffer;
	DeviceMemory* _vertexBufferMemory;

	Buffer* _indexBuffer;
	DeviceMemory* _indexBufferMemory;

	Buffer* _materialBuffer;
	DeviceMemory* _materialBufferMemory;

	Buffer* _offsetBuffer;
	DeviceMemory* _offsetBufferMemory;

	Buffer* _aabbBuffer;
	DeviceMemory* _aabbBufferMemory;

	Buffer* _proceduralBuffer;
	DeviceMemory* _proceduralBufferMemory;

	std::vector<std::unique_ptr<TextureImage>> _textureImages;
	std::vector<VkImageView> _textureImageViewHandles;
	std::vector<VkSampler> _textureSamplerHandles;
};
#endif// SCENE_H
