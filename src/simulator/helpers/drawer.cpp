//--------------------------------------------------
// Atta
// drawer.cpp
// Date: 2020-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "drawer.h"
#include "log.h"

namespace atta
{
	Drawer::Drawer(Scene* scene):
		_scene(scene)
	{

	}

	Drawer::~Drawer()
	{

	}

	void Drawer::drawPhysicsShapes()
	{
		for(auto& object : _scene->getObjects())
		{
			phy::Body* body = object->getBodyPhysics();
			if(body != nullptr)
			{
				std::vector<phy::Shape*> shapes = body->getShapes();
				for(auto& shape : shapes)
				{
					if(shape == nullptr)
						continue;

					switch(shape->getType())
					{
						case SPHERE_SHAPE:
							{
								SphereShape* bs = (SphereShape*) shape;
								//drawBox(bs->getPosition(), bs->getOrientation(), bs->getSize(), {1,0,0});
								quat q = quat(1,0,0,1);
								q.normalize();
								drawSphere({2,2,0}, q, 1.0f, {1,0,0});
							}
							break;
						case BOX_SHAPE:
							{
								BoxShape* bs = (BoxShape*) shape;
								//drawBox(bs->getPosition(), bs->getOrientation(), bs->getSize(), {1,0,0});
								quat q = quat(1,1,1,1);
								q.normalize();
								drawBox({0,3,0}, q, bs->getSize(), {1,0,0});
							}
							break;
					}
				}
			}
		}
	}

	void Drawer::drawSphere(vec3 position, quat orientation, float radius, vec3 color)
	{
		int qtyLong = 16;
		int qtyLat = 16;
		//---------- Create vertices ----------//
		std::vector<vec3> v;// Vertices
		for(int p=0; p<qtyLat+1; p++)
		{
			for(int t=0; t<qtyLong; t++)
			{
				float phi = -M_PI+p*(M_PI/qtyLat);
				float theta = t*(2*M_PI/qtyLong);
				v.push_back({radius*cos(theta)*sin(phi), radius*cos(phi), radius*sin(theta)*sin(phi)});
			}
		}

		//---------- Transform vertices ----------//
		mat4 transform = mat4();
		transform.setOrientationAndPos(orientation, position);

		for(auto& vertex : v)
		{
			vertex = transform*vertex;	
			//std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
		}

		//---------- Draw lines ----------//
		for(int j=0; j<qtyLat+1; j++)
		{
			for(int i=0; i<qtyLong; i++)
			{
				int curr = i + qtyLong*j;
				int next = (i+1)%qtyLong + qtyLong*j;
				_scene->addLine(v[curr], v[next], color);

				if(j<qtyLat)
				{
					_scene->addLine(v[curr], v[curr+qtyLong], color);
				}
			}
		}

	}

	void Drawer::drawBox(vec3 position, quat orientation, vec3 size, vec3 color)
	{
		//---------- Create vertices ----------//
		std::vector<vec3> v;// Vertices
		// Bottom
		v.push_back({-0.5, -0.5, -0.5});
		v.push_back({0.5, -0.5, -0.5});
		v.push_back({0.5, -0.5, 0.5});
		v.push_back({-0.5, -0.5, 0.5});
		// Top
		v.push_back({-0.5, 0.5, -0.5});
		v.push_back({0.5, 0.5, -0.5});
		v.push_back({0.5, 0.5, 0.5});
		v.push_back({-0.5, 0.5, 0.5});


		//---------- Transform vertices ----------//
		mat4 transform = mat4();
		transform.setOrientationAndPos(orientation, position);

		for(auto& vertex : v)
		{
			vertex = transform*vertex;	
		}

		//---------- Draw lines ----------//
		// Bottom
		_scene->addLine(v[0], v[1], color);
		_scene->addLine(v[1], v[2], color);
		_scene->addLine(v[2], v[3], color);
		_scene->addLine(v[3], v[0], color);

		// Top
		_scene->addLine(v[4], v[5], color);
		_scene->addLine(v[5], v[6], color);
		_scene->addLine(v[6], v[7], color);
		_scene->addLine(v[7], v[4], color);

		// Vertical
		_scene->addLine(v[0], v[4], color);
		_scene->addLine(v[1], v[5], color);
		_scene->addLine(v[2], v[6], color);
		_scene->addLine(v[3], v[7], color);
	}
}
