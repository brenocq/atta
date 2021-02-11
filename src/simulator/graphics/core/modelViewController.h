//--------------------------------------------------
// Robot Simulator
// modelViewController.h
// Date: 2020-07-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MODEL_VIEW_CONTROLLER_H
#define ATTA_MODEL_VIEW_CONTROLLER_H

#include <memory>
#include "simulator/graphics/vulkan/vulkan.h"
#include "simulator/graphics/core/window.h"
#include "simulator/helpers/log.h"
#include "simulator/math/math.h"

namespace atta
{
	class ModelViewController
	{
		public:
			enum ControlType
			{
				CONTROL_TYPE_2D,
				CONTROL_TYPE_3D
			};

			ModelViewController(ControlType controlType);
			~ModelViewController();

			void reset(const mat4& viewMatrix);
			mat4 getModelView() const;

			bool onKey(int key, int scancode, int action, int mods);
			bool onCursorPosition(double xpos, double ypos);
			void onMouseButton(int button, int action, int mods);
			void onScroll(double xoffset, double yoffset);

			bool updateCamera(double timeDelta);

			//---------- Getters ----------//
			vec3 getPosition() const { return _position; }
			vec3 getForward() const { return _forward; }
			vec3 getUp() const { return _up; }

		private:
			void moveForward(float d);
			void moveRight(float d);
			void moveUp(float d);
			void rotate(float y, float x);
			void updateVectors();

			ControlType _controlType;

			// Control states
			bool _cameraMovingLeft{};
			bool _cameraMovingRight{};
			bool _cameraMovingBackward{};
			bool _cameraMovingForward{};
			bool _cameraMovingDown{};
			bool _cameraMovingUp{};

			// Movement buttons
			bool _mouseMiddleButton;
			bool _shiftKey;

			float _cursorMovX{};
			float _cursorMovY{};
			float _speed;

			// Matrices and vectors.
			mat4 _orientation{};

			vec3 _position;
			vec3 _right, _up, _forward;

			double _mousePosX;
			double _mousePosY;
	};
}

#endif// ATTA_MODEL_VIEW_CONTROLLER_H
