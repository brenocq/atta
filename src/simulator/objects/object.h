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
#include <memory>
#include "simulator/math/math.h"
#include "simulator/physics/body.h"
#include "simulator/physics/constraints/constraints.h"
#include "simulator/graphics/core/model.h"

namespace atta
{
	class Object
	{
		public:
			enum ObjectSelection {
				UNSELECTED = 0,
				SELECTED
			};

			struct CreateInfo
			{
				std::string name = "Object";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;
			};


			Object(CreateInfo info);
			~Object();

			//---------- Getters ----------//
			std::string getType() const { return _type; }
			std::string getName() const { return _name; }
			bool isLight() const { return _isLight; }
			int getId() const { return _id; }
			mat4 getModelMat() const;
			quat getOrientation() const { return _orientation; }

			// Graphics
			std::shared_ptr<Model> getModel() const { return _model; }
			vec3 getPosition() const { return _position; }
			ObjectSelection getSelection() const { return _selection; }
			vec3 getScale() const { return _scale; }

			// Physics
			std::shared_ptr<phy::Body> getBodyPhysics() const { return _bodyPhysics; }

			// Object hierarchy
			//std::shared_ptr<Object> getParent() const { return _parent.lock(); }
			//std::vector<std::shared_ptr<Object>> getChildren() const { return _children; }
			//std::shared_ptr<atta::phy::Constraint> getParentConstraint() const { return _parentConstraint; }


			//---------- Setters ----------//
			void setPosition(vec3 position) { _position = position; }
			void setOrientation(quat orientation) { _orientation = orientation; }

		protected:
			//void setParent(Object* parent) { _parent = parent; };
			//void setParentConstraint(atta::phy::Constraint* constraint);

			//---------- Setters ----------//
			void setType(std::string type) { _type = type; }
			void setModel(std::shared_ptr<Model> model) { _model = model; }

			//----- Identification -----//
			std::string _type;

			static int _qtyIds;
			int _id;

			std::string _name;
			bool _isLight;

			//----- Object data -----//
			vec3 _position;
			quat _orientation;
			vec3 _scale;
			
			//----- Graphics -----//
			std::shared_ptr<Model> _model;

			// UI
			ObjectSelection _selection;

			//----- Physics -----//
			std::shared_ptr<phy::Body> _bodyPhysics;
			std::weak_ptr<phy::Constraint> _parentConstraint;

			//----- Object hierarchy -----//
			std::weak_ptr<Object> _parent;
			std::vector<std::shared_ptr<Object>> _children;
	};
}

#endif// OBJECT_H
