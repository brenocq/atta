//--------------------------------------------------
// Robot Simulator
// modelViewController.h
// Date: 2020-07-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MODEL_VIEW_CONTROLLER_H
#define ATTA_MODEL_VIEW_CONTROLLER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "simulator/graphics/core/window.h"
#include "simulator/helpers/log.h"

namespace atta
{
	class ModelViewController
	{
		public:
			ModelViewController(std::shared_ptr<Window> window);
			~ModelViewController();

			void reset(const glm::mat4& modelView);
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

			std::shared_ptr<Window> _window;

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

			vec4 _position{};
			vec4 _right{ 1, 0, 0, 0 };
			vec4 _up{ 0, 1, 0, 0 };
			vec4 _forward{ 0, 0, -1, 0 };

			double _mousePosX;
			double _mousePosY;
	};
}

#endif// ATTA_MODEL_VIEW_CONTROLLER_H
