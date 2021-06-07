//--------------------------------------------------
// Robot Simulator
// modelViewController.cpp
// Date: 2020-07-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/core/modelViewController.h>
#include <stdio.h>
#include <iostream>
#include <math.h>// isnan

namespace atta
{
	ModelViewController::ModelViewController(ControlType controlType):
		_controlType(controlType), _mouseMiddleButton(false), _shiftKey(false), 
		_cameraUpdated(false),
		_speed(1.0f),
		_right(vec3(1,0,0)), _up(vec3(0,1,0)), _forward(vec3(0,0,-1))
	{
	}

	ModelViewController::~ModelViewController()
	{

	}

	void ModelViewController::reset(const mat4& viewMatrix)
	{
		const mat4 cameraToWorld = inverse(viewMatrix);

		_position = vec3(cameraToWorld.col(3));
		_orientation = mat4(mat3(cameraToWorld));

		//Log::debug("MVC", "ori:$0pos:$1", _orientation.toString(), _position.toString());
		
		_cursorMovX = 0;
		_cursorMovY = 0;

		updateVectors();
		_cameraUpdated = true;
	}

	mat4 ModelViewController::getModelView() const
	{
		//const auto view = _orientation.translate(-_position);
		mat4 res = lookAt(_position, _position+_forward, _up);
		return res;
	}

	bool ModelViewController::onKey(int key, int scancode, int action, int mods)
	{
		if(_controlType == CONTROL_TYPE_3D)
		{
			switch(key)
			{
				case GLFW_KEY_S: 
					_cameraMovingBackward = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_W: 
					_cameraMovingForward = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_A: 
					_cameraMovingLeft = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_D: 
					_cameraMovingRight = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_Q: 
					_cameraMovingDown = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_E: 
					_cameraMovingUp = action != GLFW_RELEASE; 
					return true;
				case GLFW_KEY_LEFT_SHIFT:
					_shiftKey  = action != GLFW_RELEASE;
					return false;
				default: 
					return false;
			}
		}
		//Log::debug("MVCkey", "ok");
	}

	void ModelViewController::onMouseButton(int button, int action, int mods)
	{
		switch(button)
		{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				_mouseMiddleButton = action == GLFW_PRESS;
				_cursorMovX = 0;
				_cursorMovY = 0;
				break;
		}
		//Log::debug("MVC", "MouseMiddleButton: $0", _mouseMiddleButton);
	}

	void ModelViewController::onScroll(double xoffset, double yoffset)
	{
		//moveForward(yoffset/3.f);
	}

	bool ModelViewController::onCursorPosition(double xpos, double ypos)
	{
		const auto deltaX = static_cast<float>(xpos - _mousePosX);
		const auto deltaY = static_cast<float>(ypos - _mousePosY);

		_cursorMovX += deltaX;
		_cursorMovY += deltaY;
		_mousePosX = xpos;
		_mousePosY = ypos;

		return true;
	}

	bool ModelViewController::updateCamera(double timeDelta)
	{
		const auto d = static_cast<float>(_speed * timeDelta);
		bool changed = _cameraUpdated;
		_cameraUpdated = false;

		if(_mouseMiddleButton)
		{
			if(_controlType == CONTROL_TYPE_2D)
			{
				const float speed2DFac = 0.1;
				moveRight(d*_cursorMovX*speed2DFac);
				moveUp(-d*_cursorMovY*speed2DFac);

				changed = changed || _cursorMovX!=0 || _cursorMovY!=0;
			}
			else if(_controlType == CONTROL_TYPE_3D)
			{
				if(_cameraMovingLeft) moveRight(-d);
				if(_cameraMovingRight) moveRight(d);
				if(_cameraMovingBackward) moveForward(-d);
				if(_cameraMovingForward) moveForward(d);
				if(_cameraMovingDown) moveUp(-d);
				if(_cameraMovingUp) moveUp(d);

				changed = changed || _cameraMovingLeft || _cameraMovingRight || _cameraMovingBackward ||
				_cameraMovingForward || _cameraMovingDown || _cameraMovingUp ||
				_cursorMovX!=0 || _cursorMovY!=0;

				const float rotationDiv = 300;
				rotate(_cursorMovX / rotationDiv, _cursorMovY / rotationDiv);
			}

			_cursorMovX = 0;
			_cursorMovY = 0;
		}

		return changed;
	}

	void ModelViewController::moveForward(float d)
	{
		_position += d * _forward;
	}

	void ModelViewController::moveRight(float d)
	{
		_position += d * _right;
	}

	void ModelViewController::moveUp(float d)
	{
		_position += d * _up;
	}

	void ModelViewController::rotate(float x, float y)
	{
		_orientation =
			rotationFromAxisAngle(vec3(0,-1,0), x) * 
			(_orientation * rotationFromAxisAngle(vec3(-1,0,0), y));

		updateVectors();
	}

	void ModelViewController::updateVectors()
	{
		// Given the orientation matrix, find out the x,y,z vector orientation.
		_right = _orientation * vec3(1, 0, 0);
		_up = _orientation * vec3(0, 1, 0);
		_forward = _orientation * vec3(0, 0, -1);
	}
}
