#include "camera.h"

Camera::Camera(CameraInitialState initialState):
	_initialState(initialState)
{

}

Camera::~Camera()
{

}

void Camera::reset(const glm::mat4& modelView)
{
	const auto inverse = glm::inverse(modelView);

	_position = inverse * glm::vec4(0, 0, 0, 1);
	_orientation = glm::mat4(glm::mat3(modelView));
	
	_cameraRotX = 0;
	_cameraRotY = 0;
	_modelRotX = 0;
	_modelRotY = 0;

	updateVectors();
}

glm::mat4 Camera::modelView() const
{
	const auto cameraRotX = static_cast<float>(_modelRotY / 300.0);
	const auto cameraRotY = static_cast<float>(_modelRotX / 300.0);
	
	const auto model =
		glm::rotate(glm::mat4(1.0f), cameraRotY * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), cameraRotX * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	const auto view = _orientation * glm::translate(glm::mat4(1), -glm::vec3(_position));

	return view * model;
}

bool Camera::onKey(const int key, const int scancode, const int action, const int mods)
{
	switch (key)
	{
		case GLFW_KEY_S: _cameraMovingBackward = action != GLFW_RELEASE; return true;
		case GLFW_KEY_W: _cameraMovingForward = action != GLFW_RELEASE; return true;
		case GLFW_KEY_A: _cameraMovingLeft = action != GLFW_RELEASE; return true;
		case GLFW_KEY_D: _cameraMovingRight = action != GLFW_RELEASE; return true;
		case GLFW_KEY_LEFT_CONTROL: _cameraMovingDown = action != GLFW_RELEASE; return true;
		case GLFW_KEY_LEFT_SHIFT: _cameraMovingUp = action != GLFW_RELEASE; return true;
		default: return false;
	}
}

bool Camera::onCursorPosition(const double xpos, const double ypos)
{
	const auto deltaX = static_cast<float>(xpos - _mousePosX);
	const auto deltaY = static_cast<float>(ypos - _mousePosY);

	if (_mouseLeftPressed)
	{
		_cameraRotX += deltaX;
		_cameraRotY += deltaY;
	}

	if (_mouseRightPressed)
	{
		_modelRotX += deltaX;
		_modelRotY += deltaY;
	}

	_mousePosX = xpos;
	_mousePosY = ypos;

	return _mouseLeftPressed || _mouseRightPressed;
}

bool Camera::onMouseButton(const int button, const int action, const int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		_mouseLeftPressed = action == GLFW_PRESS;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		_mouseRightPressed = action == GLFW_PRESS;
	}

	return true;
}

bool Camera::updateCamera(const double speed, const double timeDelta)
{
	const auto d = static_cast<float>(speed * timeDelta);

	if (_cameraMovingLeft) moveRight(-d);
	if (_cameraMovingRight) moveRight(d);
	if (_cameraMovingBackward) moveForward(-d);
	if (_cameraMovingForward) moveForward(d);
	if (_cameraMovingDown) moveUp(-d);
	if (_cameraMovingUp) moveUp(d);

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

void Camera::moveForward(const float d)
{
	_position += d * _forward;
}

void Camera::moveRight(const float d)
{
	_position += d * _right;
}

void Camera::moveUp(const float d)
{
	_position += d * _up;
}

void Camera::rotate(const float y, const float x)
{
	_orientation =
		glm::rotate(glm::mat4(1), x, glm::vec3(1, 0, 0)) *
		_orientation *
		glm::rotate(glm::mat4(1), y, glm::vec3(0, 1, 0));

	updateVectors();
}

void Camera::updateVectors()
{
	// Given the ortientation matrix, find out the x,y,z vector orientation.
	const auto inverse = glm::inverse(_orientation);
	
	_right = inverse * glm::vec4(1, 0, 0, 0);
	_up = inverse * glm::vec4(0, 1, 0, 0);
	_forward = inverse * glm::vec4(0, 0, -1, 0);
}
