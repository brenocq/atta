//--------------------------------------------------
// Atta Algorithms - Linear Algebra
// LU.h
// Date: 2021-04-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/math/math.h>

namespace atta::linalg
{
	class LU
	{
		public:
			mat lu;
			mat& Aref;
			size_t n;// Number of rows/columns
			vec indx;
			float d;// If number of row interchanges are even(1) or odd(-1) (used to determine the determinant sign)

			LU(mat& A);
	};
}
