//--------------------------------------------------
// Atta Graphics System
// orthographicCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <atta/fileSystem/serializer/serializer.h>

namespace atta
{
    OrthographicCamera::OrthographicCamera(CreateInfo info):
        Camera("OrthographicCamera"), _far(info.far), _height(info.height)
    {
        _position = info.position;
        _front = normalize(info.lookAt-info.position);
        _up = normalize(info.up);
        _left = cross(_front, _up);
        _up = cross(_front, _left);

        _left.normalize();
        _up.normalize();
        _control = info.control;

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

    void OrthographicCamera::serialize(std::ostream& os)
    {
        write(os, std::string("OrthographicCamera"));
        write(os, _position);
        write(os, _left);
        write(os, _up);
        write(os, _front);
        write(os, _ratio);
        write(os, _control);
        write(os, _speed);
        write(os, _far);
        write(os, _height);
    }

    void OrthographicCamera::deserialize(std::istream& is)
    {
        read(is, _position);
        read(is, _left);
        read(is, _up);
        read(is, _front);
        read(is, _ratio);
        read(is, _control);
        read(is, _speed);
        read(is, _far);
        read(is, _height);
    }
}
