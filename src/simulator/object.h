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
#include "physics/body.h"
#include "physics/constraints/constraints.h"
#include "vulkan/model.h"

using namespace atta;
using namespace phy;

class Object
{
	public:
		enum ObjectSelection {
			UNSELECTED = 0,
			SELECTED
		};

		Object(std::string name, atta::vec3 position = {0,0,0}, atta::vec3 rotation = {0,0,0}, atta::vec3 scale = {1,1,1}, float mass = 1.0f);
		~Object();

		void addChild(Object* child, atta::phy::Constraint* constraint);
		void removeChild(Object* child);

		//---------- Getters ----------//
		atta::phy::Body* getBodyPhysics() const { return _bodyPhysics; }
		atta::vec3 getPosition() const { return _position; }
		atta::vec3 getRotation();
		atta::vec3 getWorldPosition();
		atta::vec3 getWorldRotation();
		atta::mat4 getModelMat();
		std::string getType() const { return _type; }
		std::string getName() const { return _name; }
		int getId() const { return _id; }
		bool getStatic() const { return _static; }
		Object* getParent() const { return _parent; }
		std::vector<Object*> getChildren() const { return _children; }
		atta::phy::Constraint* getParentConstraint() const { return _parentConstraint; }
		Model* getModel() const { return _model; }
		ObjectSelection getSelection() const { return _selection; }

		//---------- Setters ----------//
		void setPosition(atta::vec3 position);
		void setRotation(atta::vec3 rotation);
		void setStatic(bool stat);
		void setSelection(ObjectSelection sel);

	protected:
		void setParent(Object* parent) { _parent = parent; };
		void setParentConstraint(atta::phy::Constraint* constraint);

		std::string _name;
		std::string _type;
		int _id;
		static int _qtyIds;

		atta::vec3 _position;
		atta::vec3 _rotation;
		atta::quat _orientation;
		atta::vec3 _scale;
		float _mass;

		atta::phy::Body* _bodyPhysics;
		bool _static;

		Model* _model;

		// UI
		ObjectSelection _selection;

	private:
		Object* _parent;
		atta::phy::Constraint* _parentConstraint;
		std::vector<Object*> _children;
};

#endif// OBJECT_H
