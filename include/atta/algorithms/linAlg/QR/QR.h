//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// QR.h
// Date: 2021-04-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/math.h>

namespace atta::linalg
{
	class QR
	{
		public:
			size_t m, n;// Number of rows/columns
			mat Qt, R;// Matrices Q^T and R
			bool sing;// Indicates wheter A is singular

			QR(const mat& A);

			void solve(const vec& b, vec& x);

		private: 
			void rsolve(vec& b, vec& x);
			//void update();
			//void rotate(const int i, const float a, const float b);
	};
}
