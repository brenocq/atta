//--------------------------------------------------
// Atta Algorithms - Image Processing
// fundamentalMatrix.h
// Date: 2021-04-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/math.h>

namespace atta::imgproc
{
	class FundamentalMatrix
	{
		public:
			mat3 F;// Fundamental matrix

			FundamentalMatrix();

			void solve8Points(std::vector<std::pair<vec2i, vec2i>> points);
			void solve8PointsRANSAC(std::vector<std::pair<vec2i, vec2i>> points);

		private: 
	};
}
