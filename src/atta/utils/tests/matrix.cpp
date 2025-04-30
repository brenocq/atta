//--------------------------------------------------
// Atta Math Tests
// matrix.cpp
// Date: 2025-04-12
// By: Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/math/matrix.h>
#include <cmath>
#include <gtest/gtest.h>

using namespace atta;

namespace {
// Helper: Compare two vec3 using EXPECT_NEAR for each component
void expectVec3Equal(const vec3& a, const vec3& b, float tol = 1e-5f) {
    EXPECT_NEAR(a.x, b.x, tol);
    EXPECT_NEAR(a.y, b.y, tol);
    EXPECT_NEAR(a.z, b.z, tol);
}

//===========================================================================
// Tests for mat4
//===========================================================================

TEST(Utils_Mat4, DefaultConstructor) {
    mat4 m;
    // Default constructor sets all elements to zero
    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(m.data[i], 0.0f);
    }
}

TEST(Utils_Mat4, DiagonalConstructorIdentity) {
    mat4 id(1.0f);
    // Identity matrix expected: diagonal elements 1, others 0.
    EXPECT_FLOAT_EQ(id.data[0], 1.0f);
    EXPECT_FLOAT_EQ(id.data[5], 1.0f);
    EXPECT_FLOAT_EQ(id.data[10], 1.0f);
    EXPECT_FLOAT_EQ(id.data[15], 1.0f);
    for (int i = 0; i < 16; i++) {
        // Check off-diagonals for zero
        if (i != 0 && i != 5 && i != 10 && i != 15)
            EXPECT_FLOAT_EQ(id.data[i], 0.0f);
    }
}

TEST(Utils_Mat4, CopyConstructorAndAssignment) {
    mat4 m1(1.0f);
    mat4 m2 = m1;
    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(m1.data[i], m2.data[i]);
    }

    mat4 m3;
    m3 = m1;
    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(m3.data[i], m1.data[i]);
    }
}

TEST(Utils_Mat4, MultiplicationIdentity) {
    mat4 id(1.0f);
    // Multiply identity with a translation matrix.
    vec3 trans(1.0f, 2.0f, 3.0f);
    mat4 t = posOri(trans, quat()); // quat() is identity rotation.
    mat4 res = id * t;
    // Should equal t.
    EXPECT_FLOAT_EQ(res.data[3], t.data[3]);   // x translation
    EXPECT_FLOAT_EQ(res.data[7], t.data[7]);   // y translation
    EXPECT_FLOAT_EQ(res.data[11], t.data[11]); // z translation
}

TEST(Utils_Mat4, TransformVector) {
    // Build a translation matrix and test transform on a point.
    vec3 pos(10.0f, 20.0f, 30.0f);
    // Identity quaternion gives no rotation.
    mat4 transform = posOri(pos, quat());
    vec3 point(1.0f, 1.0f, 1.0f);
    // Apply transform: since there is no rotation/scaling,
    // the result should be point + pos.
    vec3 transformed = transform.transform(point);
    expectVec3Equal(transformed, pos + point);
}

TEST(Utils_Mat4, Transpose) {
    // Create a non-symmetric matrix and test its transpose.
    mat4 m(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f);
    mat4 t = m;
    t.transpose();

    // Check that t(i,j) = m(j,i)
    EXPECT_FLOAT_EQ(t.mat[0][1], m.mat[1][0]);
    EXPECT_FLOAT_EQ(t.mat[1][2], m.mat[2][1]);
    EXPECT_FLOAT_EQ(t.mat[2][3], m.mat[3][2]);
}

TEST(Utils_Mat4, Inverse) {
    // Create a matrix that is a combination of translation and scaling,
    // and compute its inverse. Verify that multiplying a matrix by its inverse gives identity.
    vec3 pos(3.0f, -4.0f, 5.0f);
    quat rot;
    rot.setEuler(vec3(0.2f, 0.3f, 0.4f));
    vec3 scale(2.0f, 3.0f, 4.0f);
    mat4 m = posOriScale(pos, rot, scale);
    mat4 inv = m;
    inv.setInverse(m);
    mat4 identity = m * inv;

    // Check that the identity's diagonal elements are near 1 and off-diagonals near 0.
    EXPECT_NEAR(identity.data[0], 1.0f, 1e-3f);
    EXPECT_NEAR(identity.data[5], 1.0f, 1e-3f);
    EXPECT_NEAR(identity.data[10], 1.0f, 1e-3f);
    EXPECT_NEAR(identity.data[15], 1.0f, 1e-3f);
    EXPECT_NEAR(identity.data[1], 0.0f, 1e-3f);
    EXPECT_NEAR(identity.data[2], 0.0f, 1e-3f);
    EXPECT_NEAR(identity.data[3], 0.0f, 1e-3f);
}

TEST(Utils_Mat4, GetPositionOrientationScale) {
    // Set a matrix from position, orientation, and scale.
    vec3 pos(1.0f, 2.0f, 3.0f);
    quat q;
    q.setEuler(vec3(0.1f, 0.2f, 0.3f)); // Some rotation.
    vec3 scale(1.0f, 2.0f, 3.0f);
    mat4 m;
    m.setPosOriScale(pos, q, scale);
    vec3 outPos;
    quat outQ;
    vec3 outScale;
    m.getPosOriScale(outPos, outQ, outScale);

    expectVec3Equal(pos, outPos, 1e-2f);
    expectVec3Equal(scale, outScale, 1e-2f);

    // Check if quaternions match
    EXPECT_NEAR(std::abs(q.i - outQ.i), 0.0f, 1e-3f);
    EXPECT_NEAR(std::abs(q.j - outQ.j), 0.0f, 1e-3f);
    EXPECT_NEAR(std::abs(q.k - outQ.k), 0.0f, 1e-3f);
    EXPECT_NEAR(std::abs(q.r - outQ.r), 0.0f, 1e-3f);
}

TEST(Utils_Mat4, TransformPoint) {
    // Create a transformation matrix with:
    //   Translation: pos = (3,4,5)
    //   Rotation: 90 degrees about Z-axis
    //   Scale: (2, 2, 2)
    vec3 pos(3.0f, 4.0f, 5.0f);
    vec3 scale(2.0f, 2.0f, 2.0f);
    float angle = M_PI / 2.0f; // 90 degrees
    quat q;
    q.setAxisAngle(vec3(0, 0, 1), angle);
    // Build transformation matrix (pos, rotation, scale)
    mat4 transform = posOriScale(pos, q, scale);

    // Create a test point p = (1,0,0) (in world coordinates)
    vec3 p(1.0f, 0.0f, 0.0f);

    // Expected transformation:
    // 1. Scale (1,0,0) by (2,2,2) --> (2,0,0)
    // 2. Rotate (2,0,0) by 90° about Z-axis --> (0,2,0)
    // 3. Translate by (3,4,5) --> (3+0, 4+2, 5+0) = (3,6,5)
    vec3 expected(3.0f, 6.0f, 5.0f);

    // Transform p using our matrix:
    vec3 result = transform.transform(p);
    expectVec3Equal(result, expected);
}

TEST(Utils_Mat4, ConcatTransform) {
    // Construct two transformations:
    // A: Pure translation by (1, 1, 1).
    vec3 transA(1.0f, 1.0f, 1.0f);
    // Use identity rotation and scale.
    mat4 A = posOri(transA, quat()); // using identity quaternion implicitly
    // B: Transformation that scales by (2,2,2) and rotates 90° about Z,
    //    with no translation.
    vec3 transB(0.0f, 0.0f, 0.0f);
    vec3 scaleB(2.0f, 2.0f, 2.0f);
    float angle = M_PI / 2.0f; // 90 degrees rotation
    quat qB;
    qB.setAxisAngle(vec3(0, 0, 1), angle);
    mat4 B = posOriScale(transB, qB, scaleB);

    // Concatenate the transformations. Assuming operator* means:
    //   T = A * B, so that point p is transformed as: p' = A * (B * p)
    mat4 T = A * B;

    // Choose a point p
    vec3 p(1.0f, 0.0f, 0.0f);

    // Calculate expected transformation:
    // First, apply B:
    //   p scaled: (1,0,0) * (2,2,2) -> (2,0,0)
    //   Rotated by 90° about Z: (2,0,0) -> (0,2,0)
    //   B has no translation.
    // Then, apply A: simply a translation by (1,1,1):
    //   (0,2,0) + (1,1,1) = (1,3,1)
    vec3 expected(1.0f, 3.0f, 1.0f);

    vec3 result = T.transform(p);
    expectVec3Equal(result, expected);
}

//===========================================================================
// Tests for mat3
//===========================================================================

TEST(Utils_Mat3, DefaultConstructor) {
    mat3 m;
    for (int i = 0; i < 9; i++) {
        EXPECT_FLOAT_EQ(m.data[i], 0.0f);
    }
}

TEST(Utils_Mat3, DiagonalConstructorIdentity) {
    mat3 id(1.0f);
    EXPECT_FLOAT_EQ(id.data[0], 1.0f);
    EXPECT_FLOAT_EQ(id.data[4], 1.0f);
    EXPECT_FLOAT_EQ(id.data[8], 1.0f);
    for (int i = 0; i < 9; i++) {
        if (i != 0 && i != 4 && i != 8)
            EXPECT_FLOAT_EQ(id.data[i], 0.0f);
    }
}

TEST(Utils_Mat3, Multiplication) {
    // Test a simple multiplication:
    // Let m1 = [1 2 3; 4 5 6; 7 8 9] and m2 = [9 8 7; 6 5 4; 3 2 1]
    mat3 m1(1, 2, 3, 4, 5, 6, 7, 8, 9);
    mat3 m2(9, 8, 7, 6, 5, 4, 3, 2, 1);
    mat3 prod = m1 * m2;
    // Precomputed expected results:
    // prod[0] = 1*9 + 2*6 + 3*3 = 9 + 12 + 9 = 30
    // prod[1] = 1*8 + 2*5 + 3*2 = 8 + 10 + 6 = 24
    // prod[2] = 1*7 + 2*4 + 3*1 = 7 + 8 + 3 = 18
    EXPECT_NEAR(prod.data[0], 30.0f, 1e-3f);
    EXPECT_NEAR(prod.data[1], 24.0f, 1e-3f);
    EXPECT_NEAR(prod.data[2], 18.0f, 1e-3f);
}

TEST(Utils_Mat3, Transpose) {
    mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);
    mat3 t = m;
    t.transpose();
    // In the transposed matrix, element at (row=0, col=1) should be original (row=1, col=0)
    EXPECT_FLOAT_EQ(t.data[1], m.data[3]);
    EXPECT_FLOAT_EQ(t.data[3], m.data[1]);
}
} // namespace
