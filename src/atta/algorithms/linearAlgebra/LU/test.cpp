//--------------------------------------------------
// Atta Algorithms - Linear Algebra Tests
// LU.cpp
// Date: 2021-08-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/algorithms/linearAlgebra/LU/LU.h>

using namespace atta;

namespace
{
	TEST(Algorithms_LinAlg_LU, Test)
	{
		atta::mat A(3,3);
		A[0] = {1, 0, 2};
		A[1] = {4, 2, 1};
		A[2] = {3, 5, 4};

		// |1 0 2|   |1 0 0| |u u u|
		// |4 2 1| = |l 1 0|*|0 u u|
		// |3 5 4|   |l l 1| |0 0 u|

		linalg::LU lu(A);

		const float error = 10e-5;

		EXPECT_NEAR(lu.lu[0][0], 4.0f, error);
		EXPECT_NEAR(lu.lu[0][1], 2.0f, error);
		EXPECT_NEAR(lu.lu[0][2], 1.0f, error);

		EXPECT_NEAR(lu.lu[1][0], 0.75, error);
		EXPECT_NEAR(lu.lu[1][1], 3.5f, error);
		EXPECT_NEAR(lu.lu[1][2], 3.25f, error);

		EXPECT_NEAR(lu.lu[2][0], 0.25f, error);
		EXPECT_NEAR(lu.lu[2][1], -0.142857142857f, error);
		EXPECT_NEAR(lu.lu[2][2], 2.21428571429f, error);
	}
}
