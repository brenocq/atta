//--------------------------------------------------
// Atta Graphics System
// orthographicCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <imgui_internal.h>

namespace atta
{
	OrthographicCamera::OrthographicCamera(CreateInfo info):
		_far(info.far), _height(info.height)
	{
		_position = info.position;
		_front = normalize(info.lookAt-info.position);
		_up = normalize(info.up);
		_left = cross(_front, _up);
		_up = cross(_front, _left);

		_left.normalize();
		_up.normalize();

		_ratio = info.ratio;
	}

	mat4 OrthographicCamera::getView() const
	{
		return lookAt(_position, _position+_front, _up);
	}

	mat4 OrthographicCamera::getProj() const
	{
		return orthographic(_height, _ratio, _far);
	}

	void OrthographicCamera::move()
	{
        ImGuiIO& io = ImGui::GetIO();

		float x = io.MouseDelta.x*0.01f;
		float y = io.MouseDelta.y*0.01f;

		_position += x*_left + -y*_up;
	}
}
