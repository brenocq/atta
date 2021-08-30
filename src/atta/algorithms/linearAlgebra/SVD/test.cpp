//--------------------------------------------------
// Atta Algorithms - Linear Algebra Tests
// test.cpp
// Date: 2021-08-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/algorithms/linearAlgebra/SVD/SVD.h>

using namespace atta;

namespace
{
	TEST(Algorithms_LinAlg_SVD, Test)
	{
		mat A(3,3);
		A[0] = {1, 0, 2};
		A[1] = {4, 2, 1};
		A[2] = {3, 5, 4};

		// |1 0 2|   |u u u| |s s s| |v v v|T
		// |4 2 1| = |u u u|*|s s s|*|v v v|
		// |3 5 4|   |u u u| |s s s| |v v v|
		

		linalg::SVD svd(A);

		const float error = 10e-5;

		// Check U
		EXPECT_NEAR(svd.U[0][2], 0.973842577f, error);
		EXPECT_NEAR(svd.U[0][1], 0.115512936f, error);
		EXPECT_NEAR(svd.U[0][0], 0.195671654f, error);

		EXPECT_NEAR(svd.U[1][2], 0.003455325f, error);
		EXPECT_NEAR(svd.U[1][1], -0.868569633f, error);
		EXPECT_NEAR(svd.U[1][0], 0.495555095f, error);

		EXPECT_NEAR(svd.U[2][2], -0.227197481f, error);
		EXPECT_NEAR(svd.U[2][1], 0.481916541f, error);
		EXPECT_NEAR(svd.U[2][0], 0.846249225f, error);

		// Check S
		EXPECT_NEAR(svd.S[0], 8.232179854f, error);
		EXPECT_NEAR(svd.S[1], 2.403378063f, error);
		EXPECT_NEAR(svd.S[2], 1.566840362f, error);

		// Check Vt
		EXPECT_NEAR(svd.V[0][2], 0.19534308858f, error);
		EXPECT_NEAR(svd.V[0][1], -0.79596964025f, error);
		EXPECT_NEAR(svd.V[0][0], 0.57295148968f, error);
                               
		EXPECT_NEAR(svd.V[1][2], -0.72060739732f, error);
		EXPECT_NEAR(svd.V[1][1], 0.27979095417f, error);
		EXPECT_NEAR(svd.V[1][0], 0.63438316566f, error);
                               
		EXPECT_NEAR(svd.V[2][2], 0.66525638415f, error);
		EXPECT_NEAR(svd.V[2][1], 0.53679544870f, error);
		EXPECT_NEAR(svd.V[2][0], 0.51892638167f, error);
	}
}
