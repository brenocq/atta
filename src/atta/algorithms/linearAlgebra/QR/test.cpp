//--------------------------------------------------
// Atta Algorithms - Linear Algebra Tests
// test.cpp
// Date: 2021-08-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/algorithms/linearAlgebra/QR/QR.h>

using namespace atta;

namespace
{
    TEST(Algorithms_LinAlg_QR, Test)
    {
        mat A(3,3);
        A[0] = {1, 0, 2};
        A[1] = {4, 2, 1};
        A[2] = {3, 5, 4};

        // |1 0 2|   |q q q| |r r r|
        // |4 2 1| = |q q q|*|r r r|
        // |3 5 4|   |q q q| |r r r|


        linalg::QR qr(A);
        mat Q = transpose(qr.Qt);

        const float error = 10e-5;

        // Check Q
        EXPECT_NEAR(Q[0][0], -0.196116135f, error);
        EXPECT_NEAR(Q[0][1], -0.300711407f, error);
        EXPECT_NEAR(Q[0][2], 0.933333333f, error);

        EXPECT_NEAR(Q[1][0], -0.784464540f, error);
        EXPECT_NEAR(Q[1][1], -0.522976360f, error);
        EXPECT_NEAR(Q[1][2], -0.333333333f, error);

        EXPECT_NEAR(Q[2][0], -0.588348405f, error);
        EXPECT_NEAR(Q[2][1], 0.7975389495f, error);
        EXPECT_NEAR(Q[2][2], 0.1333333333f, error);

        // Check R
        mat Ap = Q*qr.R;
        EXPECT_NEAR(Ap[0][0], A[0][0], error);
        EXPECT_NEAR(Ap[0][1], A[0][1], error);
        EXPECT_NEAR(Ap[0][2], A[0][2], error);

        EXPECT_NEAR(Ap[1][0], A[1][0], error);
        EXPECT_NEAR(Ap[1][1], A[1][1], error);
        EXPECT_NEAR(Ap[1][2], A[1][2], error);

        EXPECT_NEAR(Ap[2][0], A[2][0], error);
        EXPECT_NEAR(Ap[2][1], A[2][1], error);
        EXPECT_NEAR(Ap[2][2], A[2][2], error);

        // Solving system
        //     |1 0 2|
        // A = |4 2 1|
        //     |3 5 4|
        //
        // |r r r| |x|   |q q q|T |b|
        // |r r r|*|x| = |q q q| *|b|
        // |r r r| |x|   |q q q|  |b|
        //
        // b = [3, 3, 2]
        // x = [1, -1, 1] (unknown)
        vec b = {3, 3, 2};
        vec x = {0, 0, 0};

        qr.solve(b, x);
        EXPECT_NEAR(x[0], 1.0f, error);
        EXPECT_NEAR(x[1], -1.0f, error);
        EXPECT_NEAR(x[2], 1.0f, error);
    }
}
