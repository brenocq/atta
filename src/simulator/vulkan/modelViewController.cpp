//--------------------------------------------------
// Robot Simulator
// modelViewController.cpp
// Date: 2020-07-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "modelViewController.h"
#include <stdio.h>

ModelViewController::ModelViewController()
	//_rotateWithMouse(false)
{
}

ModelViewController::~ModelViewController()
{

}

void ModelViewController::reset(const glm::mat4& modelView)
{
	const auto inverse = glm::inverse(modelView);

	_position = inverse * glm::vec4(0, 0, 0, 1);
	_orientation = glm::mat4(glm::mat3(modelView));
	
	_cameraRotX = 0;
	_cameraRotY = 0;

	updateVectors();
}

glm::mat4 ModelViewController::getModelView() const
{
	const auto view = _orientation * glm::translate(glm::mat4(1), -glm::vec3(_position));

	return view;
}

bool ModelViewController::onKey(int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_S: 
			//_cameraMovingBackward = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_W: 
			//_cameraMovingForward = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_A: 
			//_cameraMovingLeft = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_D: 
			//_cameraMovingRight = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_Q: 
			//_cameraMovingDown = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_E: 
			//_cameraMovingUp = action != GLFW_RELEASE; 
			return true;
		case GLFW_KEY_LEFT_SHIFT:
			//_planeMoveWithMouse = true;
		default: 
			return false;
	}
}

void ModelViewController::onMouseButton(int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		//_rotateWithMouse = action == GLFW_PRESS;
	}
	//printf("Mouse button: %d %d %d (%d)\n", button, action, mods, _rotateWithMouse);
}

void ModelViewController::onScroll(double xoffset, double yoffset)
{
	moveForward(yoffset/3.f);
}

bool ModelViewController::onCursorPosition(double xpos, double ypos)
{
	const auto deltaX = static_cast<float>(xpos - _mousePosX);
	const auto deltaY = static_cast<float>(ypos - _mousePosY);

	//if(_rotateWithMouse)
	//{
	//	// Rotate with pivot point
	//	//_cameraRotX += deltaX/2;
	//	//_cameraRotY += deltaY/2;
	//	//_mousePosX = xpos;
	//	//_mousePosY = ypos;
	//}

	return true;
}

bool ModelViewController::updateCamera(double speed, double timeDelta)
{
	const auto d = static_cast<float>(speed * timeDelta);

	if(_cameraMovingLeft) moveRight(-d);
	if(_cameraMovingRight) moveRight(d);
	if(_cameraMovingBackward) moveForward(-d);
	if(_cameraMovingForward) moveForward(d);
	if(_cameraMovingDown) moveUp(-d);
	if(_cameraMovingUp) moveUp(d);

	const float rotationDiv = 300;
	rotate(_cameraRotX / rotationDiv, _cameraRotY / rotationDiv);

	const bool updated =
		_cameraMovingLeft ||
		_cameraMovingRight ||
		_cameraMovingBackward ||
		_cameraMovingForward ||
		_cameraMovingDown ||
		_cameraMovingUp ||
		_cameraRotY != 0 ||
		_cameraRotX != 0;

	_cameraRotY = 0;
	_cameraRotX = 0;

	return updated;
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
		glm::rotate(glm::mat4(1), x, glm::vec3(1, 0, 0)) *
		_orientation *
		glm::rotate(glm::mat4(1), y, glm::vec3(0, 1, 0));

	updateVectors();
}

void ModelViewController::updateVectors()
{
	// Given the orientation matrix, find out the x,y,z vector orientation.
	const auto inverse = glm::inverse(_orientation);
	
	_right = inverse * glm::vec4(1, 0, 0, 0);
	_up = inverse * glm::vec4(0, 1, 0, 0);
	_forward = inverse * glm::vec4(0, 0, -1, 0);
}

