//--------------------------------------------------
// Robot Simulator
// scene.cpp
// Date: 16/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::addModel(Model* model)
{
	_models.push_back(model);
}
