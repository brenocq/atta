//--------------------------------------------------
// Robot Simulator
// modelViewController.cpp
// Date: 2020-07-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "modelViewController.h"
#include <stdio.h>
#include <iostream>
#include <math.h>// isnan

namespace atta
{
	ModelViewController::ModelViewController():
		_mouseMiddleButton(false), _shiftKey(false), _speed(10.0f),
		_right(vec3(1,0,0)), _up(vec3(0,1,0)), _forward(vec3(0,0,-1))
	{
	}

	ModelViewController::~ModelViewController()
	{

	}

	void ModelViewController::reset(const mat4& viewMatrix)
	{
		//Log::debug("MVC", "View: $0", viewMatrix.toString());
		const mat4 cameraToWorld = inverse(viewMatrix);
		//Log::debug("MVC", "Inverse: $0", cameraToWorld.toString());

		_position = vec3(cameraToWorld.col(3));
		_orientation = mat4(mat3(viewMatrix));
		//Log::debug("MVC", "Reset: $0", _orientation.toString());
		
		_cursorMovX = 0;
		_cursorMovY = 0;

		updateVectors();
	}

	mat4 ModelViewController::getModelView() const
	{
		const auto view = _orientation.translate(-_position);
		//Log::debug("MVC", "orientation: $0", _orientation.toString());
		//Log::debug("MVC", "view: $0", view.toString());

		//mat4 res = mat4::baseAndPos(_orientation.col(0),_orientation.col(1),_orientation.col(2), -_position);
		//res.data[14]=res.data[11];
		//res.data[11]=0;
		//Log::debug("MVC", "Pos: $0, Fwd", _position.toString(), (_position+_forward).toString());
		mat4 res = lookAt(_position, _position+_forward, vec3(0,1,0));
		//Log::debug("MVC", "InvLookAt: $0", inverse(res).toString());
		return res;
	}

	bool ModelViewController::onKey(int key, int scancode, int action, int mods)
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
		//Log::debug("MVCkey", "ok");
	}

	void ModelViewController::onMouseButton(int button, int action, int mods)
	{
		switch(button)
		{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				_mouseMiddleButton = action == GLFW_PRESS;
				break;
		}
		//Log::debug("MVC", "MouseMiddleButton: $0", _mouseMiddleButton);
	}

	void ModelViewController::onScroll(double xoffset, double yoffset)
	{
		moveForward(yoffset/3.f);
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

		if(_mouseMiddleButton)
		{
			if(_cameraMovingLeft) moveRight(-d);
			if(_cameraMovingRight) moveRight(d);
			if(_cameraMovingBackward) moveForward(-d);
			if(_cameraMovingForward) moveForward(d);
			if(_cameraMovingDown) moveUp(-d);
			if(_cameraMovingUp) moveUp(d);

			const float rotationDiv = 300;
			rotate(_cursorMovX / rotationDiv, _cursorMovY / rotationDiv);
		}

		_cursorMovX = 0;
		_cursorMovY = 0;

		return true;
	}

	void ModelViewController::moveForward(float d)
	{
		//Log::debug("MVC", "forwad $0", _forward.toString());
		//Log::debug("MVC", "Old $0 - New $1", _position.toString(), (_forward).toString());
		_position -= d * _forward;
	}

	void ModelViewController::moveRight(float d)
	{
		_position += d * _right;
	}

	void ModelViewController::moveUp(float d)
	{
		_position += d * _up;
	}

	void ModelViewController::rotate(float y, float x)
	{
		//Log::debug("MVC", "first: $0", _orientation.toString());
		//Log::debug("MVC", "rot x: $0", rotationFromAxisAngle(vec3(0,1,0), y).toString());
		//Log::debug("MVC", "second: $0", (_orientation*rotationFromAxisAngle(vec3(0,1,0), y)).toString());
		//Log::debug("MVC", "third: $0", (rotationFromAxisAngle(vec3(1,0,0), x)*(_orientation*rotationFromAxisAngle(vec3(0,1,0), y))).toString());

		//_orientation =
		//	rotationFromAxisAngle(vec3(1,0,0), x) *
		//	(_orientation *
		//	rotationFromAxisAngle(vec3(0,1,0), y));

		_orientation =
			rotationFromAxisAngle(vec3(0,-1,0), y) * _orientation * rotationFromAxisAngle(vec3(1,0,0), x);

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
