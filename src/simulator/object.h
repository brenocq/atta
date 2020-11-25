//--------------------------------------------------
// Robot Simulator
// object.h
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include "physics/objectPhysics.h"
#include "physics/constraints/constraints.h"
#include "vulkan/model.h"

class Object
{
	public:
		Object(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {1,1,1}, float mass = 1.0f);
		~Object();

		void addChild(Object* child, Constraint* constraint);
		void removeChild(Object* child);

		//---------- Getters ----------//
		ObjectPhysics* getObjectPhysics() const { return _physics; }
		glm::vec3 getPosition() const { return _position; }
		glm::vec3 getRotation();
		glm::vec3 getWorldPosition();
		glm::vec3 getWorldRotation();
		glm::mat4 getModelMat();
		std::string getType() const { return _type; }
		std::string getName() const { return _name; }
		int getId() const { return _id; }
		bool getStatic() const { return _static; }
		Object* getParent() const { return _parent; }
		std::vector<Object*> getChildren() const { return _children; }
		Constraint* getParentConstraint() const { return _parentConstraint; }
		Model* getModel() const { return _model; }

		//---------- Setters ----------//
		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setStatic(bool stat);

	protected:
		void setParent(Object* parent) { _parent = parent; };
		void setParentConstraint(Constraint* constraint);

		std::string _name;
		std::string _type;
		int _id;
		static int _qtyIds;

		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;
		float _mass;

		ObjectPhysics* _physics;
		bool _static;

		Model* _model;

	private:
		Object* _parent;
		Constraint* _parentConstraint;
		std::vector<Object*> _children;
};

#endif// OBJECT_H
