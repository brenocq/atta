//--------------------------------------------------
// Atta Math Tests
// quaternion.cpp
// Date: 2025-04-12
// By: Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/math/quaternion.h>
#include <gtest/gtest.h>

using namespace atta;

namespace {
TEST(Utils_Quaternion, DefaultConstructor) {
    quat q;
    // Default quaternion should be the identity: (1, 0, 0, 0)
    EXPECT_FLOAT_EQ(q.r, 1.0f);
    EXPECT_FLOAT_EQ(q.i, 0.0f);
    EXPECT_FLOAT_EQ(q.j, 0.0f);
    EXPECT_FLOAT_EQ(q.k, 0.0f);
}

TEST(Utils_Quaternion, Normalization) {
    quat q(0.0f, 1.0f, 2.0f, 3.0f);
    q.normalize();
    float norm = std::sqrt(q.r * q.r + q.i * q.i + q.j * q.j + q.k * q.k);
    EXPECT_NEAR(norm, 1.0f, 1e-5f);
}

TEST(Utils_Quaternion, Inverse) {
    quat q(0.7071f, 0.7071f, 0.0f, 0.0f);
    quat qCopy = q;
    qCopy.inverse();
    quat identity = q * qCopy;
    // The product of a quaternion and its inverse should be approximately the identity quaternion.
    EXPECT_NEAR(identity.r, 1.0f, 1e-4f);
    EXPECT_NEAR(identity.i, 0.0f, 1e-4f);
    EXPECT_NEAR(identity.j, 0.0f, 1e-4f);
    EXPECT_NEAR(identity.k, 0.0f, 1e-4f);
}

TEST(Utils_Quaternion, Multiplication) {
    // Using two quaternions that represent 90-degree rotations around different axes.
    quat q1(0.7071f, 0.7071f, 0.0f, 0.0f);
    quat q2(0.7071f, 0.0f, 0.7071f, 0.0f);
    q1.normalize();
    q2.normalize();
    quat q3 = q1 * q2;
    // Expected result: (0.5, 0.5, 0.5, 0.5)
    EXPECT_NEAR(q3.r, 0.5f, 1e-3f);
    EXPECT_NEAR(q3.i, 0.5f, 1e-3f);
    EXPECT_NEAR(q3.j, 0.5f, 1e-3f);
    EXPECT_NEAR(q3.k, 0.5f, 1e-3f);
}

TEST(Utils_Quaternion, EulerConversion) {
    // Test converting Euler angles to quaternion and back
    vec3 euler(0.5f, 0.6f, 0.7f);
    quat q;
    q.setEuler(euler);
    vec3 euler2 = q.getEuler();
    // Allow some tolerance because of potential numerical differences and wrapping.
    EXPECT_NEAR(euler.x, euler2.x, 1e-3f);
    EXPECT_NEAR(euler.y, euler2.y, 1e-3f);
    EXPECT_NEAR(euler.z, euler2.z, 1e-3f);
}

TEST(Utils_Quaternion, RotateVector) {
    // Rotate a vector by a quaternion representing a 90-degree rotation about the Z axis.
    float angle = M_PI / 2.0f; // 90 degrees in radians
    quat q;
    q.setAxisAngle(vec3(0, 0, 1), angle);
    vec3 v(1, 0, 0);
    q.rotateVector(v);
    // The expected rotated vector should be approximately (0, 1, 0)
    EXPECT_NEAR(v.x, 0.0f, 1e-3f);
    EXPECT_NEAR(v.y, 1.0f, 1e-3f);
    EXPECT_NEAR(v.z, 0.0f, 1e-3f);
}

TEST(Utils_Quaternion, AxisAngleConversion) {
    // Create a quaternion from an axis-angle representation and then extract the axis and angle.
    vec3 axis(1, 0, 0);
    float angle = M_PI / 4.0f; // 45 degrees
    quat q;
    q.setAxisAngle(axis, angle);
    vec3 resAxis;
    float resAngle;
    q.getAxisAngle(resAxis, resAngle);
    // The axis should be (1,0,0) and the angle should be close to 45 degrees.
    EXPECT_NEAR(std::abs(resAxis.x), 1.0f, 1e-3f);
    EXPECT_NEAR(std::abs(resAxis.y), 0.0f, 1e-3f);
    EXPECT_NEAR(std::abs(resAxis.z), 0.0f, 1e-3f);
    EXPECT_NEAR(resAngle, angle, 1e-3f);
}

TEST(Utils_Quaternion, Negation) {
    quat q(0.5f, 0.5f, 0.5f, 0.5f);
    quat negQ = -q;
    EXPECT_NEAR(negQ.r, -0.5f, 1e-3f);
    EXPECT_NEAR(negQ.i, -0.5f, 1e-3f);
    EXPECT_NEAR(negQ.j, -0.5f, 1e-3f);
    EXPECT_NEAR(negQ.k, -0.5f, 1e-3f);
}

TEST(Utils_Quaternion, AddScaledVector) {
    // Start with the identity quaternion and add a small rotation via a scaled vector.
    quat q;
    vec3 smallRotation(0.1f, 0.2f, 0.3f);
    float scale = 0.05f;
    q.addScaledVector(smallRotation, scale);
    // The quaternion should now differ slightly from the identity.
    // We check that its normalized result is still a unit quaternion.
    q.normalize();
    float norm = std::sqrt(q.r * q.r + q.i * q.i + q.j * q.j + q.k * q.k);
    EXPECT_NEAR(norm, 1.0f, 1e-5f);
}

TEST(Utils_Quaternion, TwoDRotation) {
    // Set a 2D angle (rotation about Z)
    float angle = M_PI / 4.0f; // 45 degrees
    quat q;
    q.set2DAngle(angle);
    // get2DAngle should return the same angle (within tolerance)
    float recovered = q.get2DAngle();
    EXPECT_NEAR(recovered, angle, 1e-3f);

    // Additionally, test rotation of a 2D vector (ignoring z)
    vec3 v(1.0f, 0.0f, 0.0f);
    q.rotateVector(v);
    // A 45° rotation about Z should rotate (1,0,0) to (sqrt(2)/2, sqrt(2)/2, 0)
    vec3 expected(std::sqrt(2.0f) / 2, std::sqrt(2.0f) / 2, 0.0f);
    EXPECT_NEAR(v.x, expected.x, 1e-3f);
    EXPECT_NEAR(v.y, expected.y, 1e-3f);
    EXPECT_NEAR(v.z, expected.z, 1e-3f);
}

TEST(Utils_Quaternion, RotationFromVectors) {
    // Define two vectors: rotate from (1,0,0) to (0,1,0)
    vec3 before(1.0f, 0.0f, 0.0f);
    vec3 after(0.0f, 1.0f, 0.0f);
    quat q;
    q.setRotationFromVectors(before, after);

    // Expected: rotation of 90 degrees (PI/2) about the Z-axis.
    float expectedAngle = M_PI / 2.0f;
    vec3 expectedAxis(0.0f, 0.0f, 1.0f);
    quat expected;
    expected.setAxisAngle(expectedAxis, expectedAngle);

    // Compare the quaternions for equivalence (remember, q and -q represent the same rotation)
    EXPECT_NEAR(q.r, expected.r, 1e-3f);
    EXPECT_NEAR(q.i, expected.i, 1e-3f);
    EXPECT_NEAR(q.j, expected.j, 1e-3f);
    EXPECT_NEAR(q.k, expected.k, 1e-3f);

    // Alternatively, compare their rotation matrices:
    mat3 m1 = q.getRotationMatrix();
    mat3 m2 = expected.getRotationMatrix();
    for (int i = 0; i < 9; i++)
        EXPECT_NEAR(m1.data[i], m2.data[i], 1e-3f);
}
} // namespace
