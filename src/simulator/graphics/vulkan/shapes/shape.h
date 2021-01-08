//--------------------------------------------------
// Robot Simulator
// shape.h
// Date: 2020-12-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SHAPE_H
#define ATTA_SHAPE_H

namespace atta
{
	class Shape
	{
		public:
			virtual bnd3 objectBound() const = 0;
			bnd3 worldBound() const;


			const mat4* objectToWorld;
			const mat4* worldToObject;
			const bool reverseOrientation;
			const bool transformSwapsHandedness;

		private:
	};
}

#endif// ATTA_SHAPE_H
