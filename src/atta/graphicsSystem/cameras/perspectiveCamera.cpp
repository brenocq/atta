//--------------------------------------------------
// Atta Graphics System
// perspectiveCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>
#include <imgui_internal.h>

namespace atta
{
    PerspectiveCamera::PerspectiveCamera(CreateInfo info):
        _near(info.near), _far(info.far), _fov(info.fov),
        _speed(10)
    {
        _position = info.position;
        _front = normalize(info.lookAt-info.position);
        _up = normalize(info.up);
        _left = cross(_up, _front);
        _up = cross(_front, _left);

        _left.normalize();
        _up.normalize();

        _ratio = info.ratio;
    }

    mat4 PerspectiveCamera::getView() const
    {
        return lookAt(_position, _position+_front, _up);
    }

    mat4 PerspectiveCamera::getProj() const
    {
        mat4 proj = perspective(_fov, _ratio, _near, _far);
        proj.mat[1][1] *= -1;
        return proj;
    }

    void PerspectiveCamera::move()
    {
        static float lastTimeFront = 0;
        static float lastTimeLeft = 0;
        static float lastTimeUp = 0;

        ImGuiIO& io = ImGui::GetIO();

        float x = io.MouseDelta.x*0.01f;
        float y = io.MouseDelta.y*0.01f;

        mat4 orientation = mat4(1.0f);
        orientation = rotationFromAxisAngle(-_up, x) * 
            rotationFromAxisAngle(_left, y);

        _front = orientation*_front;
        _up = vec3(0,0,1);

        // Move front/back
        if(ImGui::IsKeyDown('W') || ImGui::IsKeyDown('S'))
        {
            char key = 0;
            if(io.KeysDownDuration['W'] > 0 && io.KeysDownDuration['S']>0)
                if(io.KeysDownDuration['W'] < io.KeysDownDuration['S'])
                    key = 'W';
                else
                    key = 'S';
            else if(io.KeysDownDuration['W'] > 0)
                key = 'W';
            else if(io.KeysDownDuration['S'] > 0)
                key = 'S';

            if(key)
            {
                if(io.KeysDownDuration[key] < lastTimeFront)
                    lastTimeFront = 0;

                float delta = (io.KeysDownDuration[key] - lastTimeFront) * (key == 'W'?1:-1) * _speed;
                lastTimeFront = io.KeysDownDuration[key];
                _position += _front*delta;
            }
        }

        // Move left/right
        if(ImGui::IsKeyDown('A') || ImGui::IsKeyDown('D'))
        {
            char key = 0;
            if(io.KeysDownDuration['A'] > 0 && io.KeysDownDuration['D']>0)
                if(io.KeysDownDuration['A'] < io.KeysDownDuration['D'])
                    key = 'A';
                else
                    key = 'D';
            else if(io.KeysDownDuration['A'] > 0)
                key = 'A';
            else if(io.KeysDownDuration['D'] > 0)
                key = 'D';

            if(key)
            {
                if(io.KeysDownDuration[key] < lastTimeLeft)
                    lastTimeLeft = 0;

                float delta = (io.KeysDownDuration[key] - lastTimeLeft) * (key == 'A'?1:-1) * _speed;
                lastTimeLeft = io.KeysDownDuration[key];
                _position += _left*delta;
            }
        }

        // Move up/down
        if(ImGui::IsKeyDown('E') || ImGui::IsKeyDown('Q'))
        {
            char key = 0;
            if(io.KeysDownDuration['E'] > 0 && io.KeysDownDuration['Q']>0)
                if(io.KeysDownDuration['E'] < io.KeysDownDuration['Q'])
                    key = 'E';
                else
                    key = 'Q';
            else if(io.KeysDownDuration['E'] > 0)
                key = 'E';
            else if(io.KeysDownDuration['Q'] > 0)
                key = 'Q';

            if(key)
            {
                if(io.KeysDownDuration[key] < lastTimeUp)
                    lastTimeUp = 0;

                float delta = (io.KeysDownDuration[key] - lastTimeUp) * (key == 'E'?1:-1) * _speed;
                lastTimeUp = io.KeysDownDuration[key];
                _position += _up*delta;
            }
        }

        Camera::update();
    }
}
