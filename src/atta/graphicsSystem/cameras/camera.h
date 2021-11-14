//--------------------------------------------------
// Atta Graphics System
// camera.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_CAMERAS_CAMERA_H
#define ATTA_GRAPHICS_SYSTEM_CAMERAS_CAMERA_H
#include <atta/core/math/math.h>
#include <atta/fileSystem/serializer/serializable.h>

namespace atta
{
    class Camera : public Serializable
    {
    public:
        Camera(std::string name): _name(name) {}
        virtual ~Camera() = default;

        virtual mat4 getView() const = 0;
        virtual mat4 getProj() const = 0;

        void move();

        enum class Control
        {
            PLANAR = 0,// Left click and move with mouse, scroll to zoom in/zoom out
            FIRST_PERSON,// Rotate with mouse, move with wasdqe (pressing middle button)
        };

        vec3 getPosition() const { return _position; }
        vec3 getLeft() const { return _left; }
        vec3 getUp() const { return _up; }
        vec3 getFront() const { return _front; }
        float getRatio() const { return _ratio; }
        std::string getName() const { return _name; }

        void setPosition(const vec3& position) { _position = position; }
        void setLeft(const vec3& left) { _left = left; }
        void setUp(const vec3& up) { _up = up; }
        void setFront(const vec3& front) { _front = front; }
        void update();

        void setRatio(float ratio) { _ratio = ratio; }
        virtual void renderUI();

    protected:
        vec3 _position;
        vec3 _left, _up, _front;
        float _ratio;
        Control _control;

        // Camera move
        float _speed;

        // UI
        std::string _name;
    private:
        void movePlanar();
        void moveFirstPerson();
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_CAMERAS_CAMERA_H
