//--------------------------------------------------
// Atta Algorithms - Computer Vision
// bundleAdjustment.h
// Date: 2021-04-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/math.h>

namespace atta::cv
{
	class BundleAdjustment
	{
		public:
			struct Measurement {
				size_t imageIndex;
				size_t point3DIndex;
				vec2 point;// Image 2D point
			};
			BundleAdjustment();

			void run();
			void clear();

			//---------- Setters ----------//
			void set3DPoints(std::vector<vec3> points3D) { _points3D = points3D; }
			void addImage(vec cameraPose, std::vector<std::pair<size_t, vec2>> measurements);

		private: 
			std::vector<vec> _cameraPoses;// Camera pose for each image
			std::vector<vec3> _points3D;// 3D points being projected to images
			std::vector<Measurement> _measurements;// 2D points measurements for each image
	};
}
