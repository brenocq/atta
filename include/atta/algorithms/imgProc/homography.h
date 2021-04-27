//--------------------------------------------------
// Atta Algorithms - Image Processing
// homography.h
// Date: 2021-04-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/math.h>

namespace atta::imgproc
{
	class Homography
	{
		public:
			mat3 H;

			Homography();

			void solveDLT(std::vector<std::pair<vec2i, vec2i>> points);
			void solveRANSAC(std::vector<std::pair<vec2i, vec2i>> points, size_t width, size_t height);

		private: 
	};
}
