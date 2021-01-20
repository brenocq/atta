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
		_mouseMiddleButton(false), _shiftKey(false), _speed(5.0f),
		_right(vec3(1,0,0)), _up(vec3(0,1,0)), _forward(vec3(0,0,-1))
	{
	}

	ModelViewController::~ModelViewController()
	{

	}

	void ModelViewController::reset(const mat4& modelView)
	{
		const mat4 inverse = modelView.inverse();

		_position = vec3(inverse.getCol(2));
		_orientation = mat4(mat3(modelView));
		
		_cursorMovX = 0;
		_cursorMovY = 0;

		updateVectors();
	}

	mat4 ModelViewController::getModelView() const
	{
		const auto view = _orientation * mat4(1).translate(-_position);

		return view;
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
	}

	void ModelViewController::onMouseButton(int button, int action, int mods)
	{
		switch(button)
		{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				_mouseMiddleButton = action == GLFW_PRESS;
				break;
		}
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

		// Testing movement like solidworks
		//if(_mouseMiddleButton && _shiftKey)
		//{
		//	// Displacement movement
		//	// TODO get window size
		//	glm::vec4 movement = -_right*(float)(1200/2-_mousePosX) + _up*(float)(900/2-_mousePosY);
		//	//glm::vec4 movement = _right*_cursorMovX - _up*_cursorMovY;
		//	movement = glm::normalize(movement);
		//	if(!isnan(movement[0]))
		//		_position += d * movement;
		//}
		//else if(_mouseMiddleButton)
		//{
		//	// Rotational movement
		////	glm::vec4 movement = _right*_cursorMovX - _up*_cursorMovY;
		////	movement = glm::normalize(movement);
		////	if(!isnan(movement[0]))
		////		_position += d * movement;
		//	rotate(_cursorMovX/300.0, _cursorMovY/300.0);
		//}
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

	void ModelViewController::rotate(float y, float x)
	{
		_orientation =
			mat4(1).rotate(x, vec3(1,0,0)) *
			_orientation *
			mat4(1).rotate(y, vec3(0,1,0));

		updateVectors();
	}

	void ModelViewController::updateVectors()
	{
		// Given the orientation matrix, find out the x,y,z vector orientation.
		const auto inverse = atta::inverse(_orientation);
		
		_right = inverse * vec3(1, 0, 0);
		_up = inverse * vec3(0, 1, 0);
		_forward = inverse * vec3(0, 0, -1);
	}
}
