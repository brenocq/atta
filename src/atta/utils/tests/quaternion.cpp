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

    // Test normalization of near-zero quaternion
    quat qZero(0.0f, 0.0f, 0.0f, 0.0f);
    qZero.normalize();
    EXPECT_FLOAT_EQ(qZero.r, 1.0f);
    EXPECT_FLOAT_EQ(qZero.i, 0.0f);
    EXPECT_FLOAT_EQ(qZero.j, 0.0f);
    EXPECT_FLOAT_EQ(qZero.k, 0.0f);
}

TEST(Utils_Quaternion, Invert) {
    quat q(0.7071f, 0.7071f, 0.0f, 0.0f); // Normalized 90deg rotation around X
    quat qInv = q;
    qInv.invert();
    quat identity = q * qInv;
    // The product of a quaternion and its inverse should be approximately the identity quaternion
    EXPECT_NEAR(identity.r, 1.0f, 1e-6f);
    EXPECT_NEAR(identity.i, 0.0f, 1e-6f);
    EXPECT_NEAR(identity.j, 0.0f, 1e-6f);
    EXPECT_NEAR(identity.k, 0.0f, 1e-6f);

    // Test inverse of near-zero quaternion (should become identity)
    quat qZero(0.0f, 0.0f, 0.0f, 0.0f);
    qZero.invert();
    EXPECT_FLOAT_EQ(qZero.r, 1.0f);
    EXPECT_FLOAT_EQ(qZero.i, 0.0f);
    EXPECT_FLOAT_EQ(qZero.j, 0.0f);
    EXPECT_FLOAT_EQ(qZero.k, 0.0f);
}

TEST(Utils_Quaternion, Multiplication) {
    // q1: 90 degrees around X axis
    quat q1;
    q1.setAxisAngle(vec3(1.0f, 0.0f, 0.0f), M_PI / 2.0f);
    // q2: 90 degrees around Y axis
    quat q2;
    q2.setAxisAngle(vec3(0.0f, 1.0f, 0.0f), M_PI / 2.0f);

    quat q3 = q1 * q2;
    // Expected result from q1*q2 (verified calculation): (0.5, 0.5, 0.5, 0.5)
    EXPECT_NEAR(q3.r, 0.5f, 1e-6f);
    EXPECT_NEAR(q3.i, 0.5f, 1e-6f);
    EXPECT_NEAR(q3.j, 0.5f, 1e-6f);
    EXPECT_NEAR(q3.k, 0.5f, 1e-6f);
}

TEST(Utils_Quaternion, EulerConversion) {
    vec3 euler(0.5f, 0.6f, 0.7f); // ZYX order assumed by implementation
    quat q;
    q.setEuler(euler);
    vec3 euler2 = q.getEuler();

    // Test near standard range
    EXPECT_NEAR(euler.x, euler2.x, 1e-6f);
    EXPECT_NEAR(euler.y, euler2.y, 1e-6f);
    EXPECT_NEAR(euler.z, euler2.z, 1e-6f);

    // Test gimbal lock case (pitch near +/- 90 degrees)
    vec3 eulerGimbal(0.5f, M_PI / 2.0f * 0.9999, 0.7f);
    q.setEuler(eulerGimbal);
    euler2 = q.getEuler();
    // In gimbal lock, only the sum/difference of yaw and roll might be preserved
    // Reconstruct rotation matrix to check equivalence is more robust here,
    // but let's check if the direct conversion is numerically stable near lock.
    EXPECT_NEAR(eulerGimbal.x, euler2.x, 1e-3f); // Looser tolerance near gimbal lock
    EXPECT_NEAR(eulerGimbal.y, euler2.y, 1e-3f);
    EXPECT_NEAR(eulerGimbal.z, euler2.z, 1e-3f);

    // Test specific angles
    vec3 euler90Z(0.0f, 0.0f, M_PI / 2.0f);
    q.setEuler(euler90Z);
    euler2 = q.getEuler();
    EXPECT_NEAR(euler90Z.x, euler2.x, 1e-6f);
    EXPECT_NEAR(euler90Z.y, euler2.y, 1e-6f);
    EXPECT_NEAR(euler90Z.z, euler2.z, 1e-6f);
}

TEST(Utils_Quaternion, RotateVector) {
    // Rotate a vector by a quaternion representing a 90-degree rotation about the Z axis.
    float angle = M_PI / 2.0f; // 90 degrees in radians
    quat q;
    q.setAxisAngle(vec3(0, 0, 1), angle);
    vec3 v(1, 0, 0);
    q.rotateVector(v);
    // The expected rotated vector should be approximately (0, 1, 0)
    EXPECT_NEAR(v.x, 0.0f, 1e-6f);
    EXPECT_NEAR(v.y, 1.0f, 1e-6f);
    EXPECT_NEAR(v.z, 0.0f, 1e-6f);

    // 45-degree rotation about the X axis
    angle = M_PI / 4.0f;
    q.setAxisAngle(vec3(1, 0, 0), angle);
    v = vec3(0, 1, 0);
    q.rotateVector(v);
    // Expected: (0, sqrt(2)/2, sqrt(2)/2)
    EXPECT_NEAR(v.x, 0.0f, 1e-6f);
    EXPECT_NEAR(v.y, std::sqrt(2.0f) / 2.0f, 1e-6f);
    EXPECT_NEAR(v.z, std::sqrt(2.0f) / 2.0f, 1e-6f);
}

TEST(Utils_Quaternion, AxisAngleConversion) {
    // Create a quaternion from an axis-angle representation and then extract the axis and angle.
    vec3 axis(1, 1, 1);
    axis.normalize();
    float angle = M_PI / 4.0f; // 45 degrees
    quat q;
    q.setAxisAngle(axis, angle);
    vec3 resAxis;
    float resAngle;
    q.getAxisAngle(resAxis, resAngle);
    // The axis direction should be the same and the angle should be close to 45 degrees.
    EXPECT_NEAR(resAxis.x, axis.x, 1e-3f);
    EXPECT_NEAR(resAxis.y, axis.y, 1e-3f);
    EXPECT_NEAR(resAxis.z, axis.z, 1e-3f);
    EXPECT_NEAR(resAngle, angle, 1e-3f);
}

TEST(Utils_Quaternion, Negation) {
    quat q(0.5f, 0.1f, -0.2f, 0.8f);
    quat negQ = -q;
    EXPECT_FLOAT_EQ(negQ.r, -q.r);
    EXPECT_FLOAT_EQ(negQ.i, -q.i);
    EXPECT_FLOAT_EQ(negQ.j, -q.j);
    EXPECT_FLOAT_EQ(negQ.k, -q.k);
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
    // Case 1: 90 degrees rotation Z-axis
    vec3 before(1.0f, 0.0f, 0.0f);
    vec3 after(0.0f, 1.0f, 0.0f);
    quat q;
    q.setRotationFromVectors(before, after);

    quat expected_q;
    expected_q.setAxisAngle(vec3(0.0f, 0.0f, 1.0f), M_PI / 2.0f);

    // Compare quaternions (check absolute dot product == 1 for same rotation)
    float dot_prod = q.r * expected_q.r + q.i * expected_q.i + q.j * expected_q.j + q.k * expected_q.k;
    EXPECT_NEAR(std::abs(dot_prod), 1.0f, 1e-6f);

    // Case 2: Opposite vectors (180 degrees rotation)
    before = vec3(1.0f, 0.0f, 0.0f);
    after = vec3(-1.0f, 0.0f, 0.0f);
    q.setRotationFromVectors(before, after);
    // Rotation should be 180 degrees around *any* axis perpendicular to 'before'.
    // Implementation chooses one based on cross product logic.
    // Let's test by rotating 'before' and see if we get 'after'.
    vec3 rotated_before = before;
    q.rotateVector(rotated_before);
    EXPECT_NEAR(rotated_before.x, after.x, 1e-6f);
    EXPECT_NEAR(rotated_before.y, after.y, 1e-6f);
    EXPECT_NEAR(rotated_before.z, after.z, 1e-6f);
    // Also check angle is PI
    vec3 axis;
    float angle;
    q.getAxisAngle(axis, angle);
    EXPECT_NEAR(angle, M_PI, 1e-6f);

    // Case 3: Same vectors (identity rotation)
    before = vec3(0.0f, 1.0f, 0.0f);
    after = vec3(0.0f, 1.0f, 0.0f);
    q.setRotationFromVectors(before, after);
    // Should result in identity quat (or stay as default identity)
    EXPECT_NEAR(q.r, 1.0f, 1e-6f);
    EXPECT_NEAR(q.i, 0.0f, 1e-6f);
    EXPECT_NEAR(q.j, 0.0f, 1e-6f);
    EXPECT_NEAR(q.k, 0.0f, 1e-6f);
}

TEST(Utils_Quaternion, SetRotationMatrix) {
    const float tolerance = 1e-6f;

    // Test Case 1: Identity Matrix
    mat3 identity_matrix; // Assuming default constructor gives identity or set manually
    identity_matrix.mat[0][0] = 1;
    identity_matrix.mat[0][1] = 0;
    identity_matrix.mat[0][2] = 0;
    identity_matrix.mat[1][0] = 0;
    identity_matrix.mat[1][1] = 1;
    identity_matrix.mat[1][2] = 0;
    identity_matrix.mat[2][0] = 0;
    identity_matrix.mat[2][1] = 0;
    identity_matrix.mat[2][2] = 1;

    quat q_identity;
    q_identity.setRotationMatrix(identity_matrix);

    EXPECT_NEAR(q_identity.r, 1.0f, tolerance);
    EXPECT_NEAR(q_identity.i, 0.0f, tolerance);
    EXPECT_NEAR(q_identity.j, 0.0f, tolerance);
    EXPECT_NEAR(q_identity.k, 0.0f, tolerance);

    // Test Case 2: 90 degrees rotation around Z axis
    quat ref_quat_z90;
    ref_quat_z90.setAxisAngle(vec3(0, 0, 1), M_PI / 2.0f);
    mat3 matrix_z90 = ref_quat_z90.getRotationMatrix();

    quat test_quat_z90;
    test_quat_z90.setRotationMatrix(matrix_z90);

    // Check resulting quaternion components are close to reference (or its negative)
    float dot_z90 =
        ref_quat_z90.r * test_quat_z90.r + ref_quat_z90.i * test_quat_z90.i + ref_quat_z90.j * test_quat_z90.j + ref_quat_z90.k * test_quat_z90.k;
    EXPECT_NEAR(std::abs(dot_z90), 1.0f, tolerance);

    // Check round trip: matrix -> quat -> matrix
    mat3 result_matrix_z90 = test_quat_z90.getRotationMatrix();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(matrix_z90.mat[i][j], result_matrix_z90.mat[i][j], tolerance);
        }
    }

    // Test Case 3: 45 degrees rotation around X axis
    quat ref_quat_x45;
    ref_quat_x45.setAxisAngle(vec3(1, 0, 0), M_PI / 4.0f);
    mat3 matrix_x45 = ref_quat_x45.getRotationMatrix();

    quat test_quat_x45;
    test_quat_x45.setRotationMatrix(matrix_x45);

    // Check resulting quaternion components are close to reference (or its negative)
    float dot_x45 =
        ref_quat_x45.r * test_quat_x45.r + ref_quat_x45.i * test_quat_x45.i + ref_quat_x45.j * test_quat_x45.j + ref_quat_x45.k * test_quat_x45.k;
    EXPECT_NEAR(std::abs(dot_x45), 1.0f, tolerance);

    // Check round trip: matrix -> quat -> matrix
    mat3 result_matrix_x45 = test_quat_x45.getRotationMatrix();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(matrix_x45.mat[i][j], result_matrix_x45.mat[i][j], tolerance);
        }
    }

    // Test Case 4: Arbitrary rotation (derived from Euler to hit different code paths)
    // Use Euler angles that don't result in simple trace > 0 or diagonal dominance
    vec3 euler_arb(1.0f, -2.0f, 0.5f); // Example angles in radians
    quat ref_quat_arb;
    ref_quat_arb.setEuler(euler_arb);
    mat3 matrix_arb = ref_quat_arb.getRotationMatrix();

    quat test_quat_arb;
    test_quat_arb.setRotationMatrix(matrix_arb);

    // Check resulting quaternion components are close to reference (or its negative)
    float dot_arb =
        ref_quat_arb.r * test_quat_arb.r + ref_quat_arb.i * test_quat_arb.i + ref_quat_arb.j * test_quat_arb.j + ref_quat_arb.k * test_quat_arb.k;
    EXPECT_NEAR(std::abs(dot_arb), 1.0f, tolerance);

    // Check round trip: matrix -> quat -> matrix
    mat3 result_matrix_arb = test_quat_arb.getRotationMatrix();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(matrix_arb.mat[i][j], result_matrix_arb.mat[i][j], tolerance);
        }
    }
}

} // namespace
