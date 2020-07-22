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

class Scene
{
	public:
	Scene();
	~Scene();

	void addModel(Model* model);
	std::vector<Model*> getModels() const { return _models; };

	private:
	std::vector<Model*> _models;
};

#endif// SCENE_H
