#ifndef CAMERA_H
#define CAMERA_H

#include "glm.h"

#define NOMINMAX
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#undef APIENTRY

struct CameraInitialState
{
	glm::mat4 ModelView;
	float FieldOfView;
	float Aperture;
	float FocusDistance;
	float ControlSpeed;
	bool GammaCorrection;
	bool HasSky;
};

class Camera 
{
	public:
	Camera(CameraInitialState initialState);
	~Camera();

	void reset(const glm::mat4& modelView);

	glm::mat4 modelView() const;

	bool onKey(int key, int scancode, int action, int mods);
	bool onCursorPosition(double xpos, double ypos);
	bool onMouseButton(int button, int action, int mods);
	bool updateCamera(double speed, double timeDelta);

	CameraInitialState initialState() const { return _initialState; }

	private:

	void moveForward(float d);
	void moveRight(float d);
	void moveUp(float d);
	void rotate(float y, float x);
	void updateVectors();

	// Matrices and vectors.
	glm::mat4 _orientation{};

	glm::vec4 _position{};
	glm::vec4 _right{ 1, 0, 0, 0 };
	glm::vec4 _up{ 0, 1, 0, 0 };
	glm::vec4 _forward{ 0, 0, -1, 0 };

	// Control states.
	bool _cameraMovingLeft{};
	bool _cameraMovingRight{};
	bool _cameraMovingBackward{};
	bool _cameraMovingForward{};
	bool _cameraMovingDown{};
	bool _cameraMovingUp{};

	float _cameraRotX{};
	float _cameraRotY{};
	float _modelRotX{};
	float _modelRotY{};
	
	double _mousePosX{};
	double _mousePosY{};
	
	bool _mouseLeftPressed{};
	bool _mouseRightPressed{};

	CameraInitialState _initialState;
};

#endif// CAMERA_H
