//--------------------------------------------------
// Atta Drawer
// drawer.cpp
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "drawer.h"

namespace atta
{
	void Drawer::addLineImpl(Line line)
	{
		if(_currNumberOfLines<_maxNumberOfLines)
		{
			_lines[_currNumberOfLines] = line;
			_currNumberOfLines++;
		}
	}

	std::vector<Drawer::Line>& Drawer::getLinesImpl()
	{
		return _lines;
	}

	void Drawer::clearImpl()
	{
		_lines.clear();
		_currNumberOfLines = 0;
	}
	//Drawer::Drawer(Scene* scene):
	//	_scene(scene)
	//{

	//}

	//Drawer::~Drawer()
	//{

	//}

	//void Drawer::drawPhysicsShapes()
	//{
	//	for(auto& object : _scene->getObjects())
	//	{
	//		phy::Body* body = object->getBodyPhysics();
	//		if(body != nullptr)
	//		{
	//			std::vector<phy::Shape*> shapes = body->getShapes();
	//			for(auto& shape : shapes)
	//			{
	//				if(shape == nullptr)
	//					continue;

	//				switch(shape->getType())
	//				{
	//					case SPHERE_SHAPE:
	//						{
	//							SphereShape* s = (SphereShape*) shape;
	//							vec3 position = body->getTransformMatrix()*s->getPosition();
	//							quat orientation = s->getOrientation()*body->getOrientation();
	//							drawSphere(position, orientation, s->getRadius(), {1,0,0});
	//						}
	//						break;
	//					case BOX_SHAPE:
	//						{
	//							BoxShape* bs = (BoxShape*) shape;
	//							vec3 position = body->getTransformMatrix()*bs->getPosition();
	//							quat orientation = bs->getOrientation()*body->getOrientation();
	//							drawBox(position, orientation, bs->getSize(), {1,0,0});
	//						}
	//						break;
	//					case CYLINDER_SHAPE:
	//						{
	//							CylinderShape* s = (CylinderShape*) shape;
	//							vec3 position = body->getTransformMatrix()*s->getPosition();
	//							quat orientation = s->getOrientation()*body->getOrientation();
	//							drawCylinder(position, orientation, s->getScale(), {1,0,0});
	//						}
	//						break;
	//					case CAPSULE_SHAPE:
	//						{
	//							CapsuleShape* s = (CapsuleShape*) shape;
	//							vec3 position = body->getTransformMatrix()*s->getPosition();
	//							quat orientation = s->getOrientation()*body->getOrientation();
	//							drawCapsule(position, orientation, s->getScale(), {1,0,0});
	//						}
	//						break;
	//				}
	//			}
	//		}
	//	}
	//}

	//void Drawer::drawSphere(vec3 position, quat orientation, float radius, vec3 color)
	//{
	//	int qtyLong = 24;
	//	int qtyLat = 24;
	//	//---------- Create vertices ----------//
	//	std::vector<vec3> v;// Vertices
	//	for(int p=0; p<qtyLat+1; p++)
	//	{
	//		for(int t=0; t<qtyLong; t++)
	//		{
	//			float phi = -M_PI+p*(M_PI/qtyLat);
	//			float theta = t*(2*M_PI/qtyLong);
	//			v.push_back({radius*cos(theta)*sin(phi), radius*cos(phi), radius*sin(theta)*sin(phi)});
	//		}
	//	}

	//	//---------- Transform vertices ----------//
	//	mat4 transform = mat4();
	//	transform.setPosOriScale(position, orientation, {radius*2, radius*2, radius*2});

	//	for(auto& vertex : v)
	//	{
	//		vertex = transform*vertex;	
	//		//std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	//	}

	//	//---------- Draw lines ----------//
	//	for(int j=0; j<qtyLat+1; j++)
	//	{
	//		for(int i=0; i<qtyLong; i++)
	//		{
	//			int curr = i + qtyLong*j;
	//			int next = (i+1)%qtyLong + qtyLong*j;
	//			_scene->addLine(v[curr], v[next], color);

	//			if(j<qtyLat)
	//			{
	//				_scene->addLine(v[curr], v[curr+qtyLong], color);
	//			}
	//		}
	//	}

	//}

	//void Drawer::drawBox(vec3 position, quat orientation, vec3 size, vec3 color)
	//{
	//	//---------- Create vertices ----------//
	//	std::vector<vec3> v;// Vertices
	//	// Bottom
	//	v.push_back({-0.5, -0.5, -0.5});
	//	v.push_back({0.5, -0.5, -0.5});
	//	v.push_back({0.5, -0.5, 0.5});
	//	v.push_back({-0.5, -0.5, 0.5});
	//	// Top
	//	v.push_back({-0.5, 0.5, -0.5});
	//	v.push_back({0.5, 0.5, -0.5});
	//	v.push_back({0.5, 0.5, 0.5});
	//	v.push_back({-0.5, 0.5, 0.5});


	//	//---------- Transform vertices ----------//
	//	mat4 transform = mat4();
	//	transform.setPosOriScale(position, orientation, size);

	//	for(auto& vertex : v)
	//	{
	//		vertex = transform*vertex;	
	//	}

	//	//---------- Draw lines ----------//
	//	// Bottom
	//	_scene->addLine(v[0], v[1], color);
	//	_scene->addLine(v[1], v[2], color);
	//	_scene->addLine(v[2], v[3], color);
	//	_scene->addLine(v[3], v[0], color);

	//	// Top
	//	_scene->addLine(v[4], v[5], color);
	//	_scene->addLine(v[5], v[6], color);
	//	_scene->addLine(v[6], v[7], color);
	//	_scene->addLine(v[7], v[4], color);

	//	// Vertical
	//	_scene->addLine(v[0], v[4], color);
	//	_scene->addLine(v[1], v[5], color);
	//	_scene->addLine(v[2], v[6], color);
	//	_scene->addLine(v[3], v[7], color);
	//}

	//void Drawer::drawCylinder(vec3 position, quat orientation, vec3 size, vec3 color)
	//{
	//	int qtyFaces = 24;
	//	//---------- Create vertices ----------//
	//	std::vector<vec3> v;// Vertices
	//	// Bottom-center
	//	v.push_back({0, -0.5, 0});
	//	// Top-center
	//	v.push_back({0, 0.5, 0});
	//	// Bottom circle
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		v.push_back({0.5*cos(theta), -0.5, 0.5*sin(theta)});
	//	}
	//	// Top circle
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		v.push_back({0.5*cos(theta), 0.5, 0.5*sin(theta)});
	//	}

	//	//---------- Transform vertices ----------//
	//	mat4 transform = mat4();
	//	transform.setPosOriScale(position, orientation, size);

	//	for(auto& vertex : v)
	//	{
	//		vertex = transform*vertex;	
	//	}

	//	//---------- Draw lines ----------//
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		_scene->addLine(v[0], v[i+2], color);
	//		_scene->addLine(v[1], v[i+2+qtyFaces], color);
	//		_scene->addLine(v[i+2], v[i+2+qtyFaces], color);

	//		if(i<qtyFaces-1)
	//		{
	//			_scene->addLine(v[i+2], v[i+3], color);
	//			_scene->addLine(v[i+2+qtyFaces], v[i+3+qtyFaces], color);
	//		}
	//	}
	//}

	//void Drawer::drawCapsule(vec3 position, quat orientation, vec3 size, vec3 color)
	//{
	//	int qtyFaces = 24;
	//	//---------- Create vertices ----------//
	//	std::vector<vec3> v;// Vertices
	//	// Bottom-center
	//	v.push_back({0, -0.5, 0});
	//	// Top-center
	//	v.push_back({0, 0.5, 0});
	//	// Bottom circle
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		v.push_back({0.6*cos(theta), -1.0, 0.6*sin(theta)});
	//	}
	//	// Top circle
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		v.push_back({0.6*cos(theta), 1.0, 0.6*sin(theta)});
	//	}

	//	//---------- Transform vertices ----------//
	//	mat4 transform = mat4();
	//	transform.setPosOriScale(position, orientation, size);

	//	for(auto& vertex : v)
	//	{
	//		vertex = transform*vertex;	
	//	}

	//	//---------- Draw lines ----------//
	//	for(int i=0; i<qtyFaces; i++)
	//	{
	//		float theta = i*(2*M_PI/qtyFaces);
	//		_scene->addLine(v[0], v[i+2], color);
	//		_scene->addLine(v[1], v[i+2+qtyFaces], color);
	//		_scene->addLine(v[i+2], v[i+2+qtyFaces], color);

	//		if(i<qtyFaces-1)
	//		{
	//			_scene->addLine(v[i+2], v[i+3], color);
	//			_scene->addLine(v[i+2+qtyFaces], v[i+3+qtyFaces], color);
	//		}
	//	}
	//}
}
