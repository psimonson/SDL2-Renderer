#include "matrix.h"
#include <math.h>

// Create an identity matrix
Matrix matrix_identity() {
    Matrix result = {0};
    for (int i = 0; i < 4; i++) {
        result.m[i][i] = 1.0f;
    }
    return result;
}

// Multiply two 4x4 matrices
Matrix matrix_multiply(const Matrix *a, const Matrix *b) {
    Matrix result = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += a->m[k][i] * b->m[j][k];
            }
        }
    }
    return result;
}

// Create a perspective projection matrix
Matrix set_perspective_projection(float fov, float aspect, float near, float far) {
    Matrix result = {0};
    float tan_half_fov = tanf(fov / 2.0f);

    result.m[0][0] = 1.0f / (aspect * tan_half_fov);
    result.m[1][1] = 1.0f / tan_half_fov;
    result.m[2][2] = -(far + near) / (far - near);
    result.m[2][3] = -1.0f;
    result.m[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

// Create an orthographic projection matrix
Matrix set_orthographic_projection(float left, float right, float bottom, float top, float near, float far) {
    Matrix result = matrix_identity();

    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (far - near);
    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -(far + near) / (far - near);

    return result;
}

// Create a view matrix (look-at matrix)
Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up) {
    Vec3 forward = vec3_normalize(vec3_sub(target, eye));
    Vec3 right = vec3_normalize(vec3_cross(up, forward));
    Vec3 recalculated_up = vec3_cross(forward, right);

    Matrix result = matrix_identity();
    result.m[0][0] = right.x;
    result.m[1][0] = right.y;
    result.m[2][0] = right.z;
    result.m[0][1] = recalculated_up.x;
    result.m[1][1] = recalculated_up.y;
    result.m[2][1] = recalculated_up.z;
    result.m[0][2] = forward.x;
    result.m[1][2] = forward.y;
    result.m[2][2] = forward.z;
    result.m[3][0] = -vec3_dot(right, eye);
    result.m[3][1] = -vec3_dot(recalculated_up, eye);
    result.m[3][2] = -vec3_dot(forward, eye);

    return result;
}

// Create a translation matrix
Matrix matrix_translate(float x, float y, float z) {
    Matrix result = matrix_identity();
    result.m[3][0] = x;
    result.m[3][1] = y;
    result.m[3][2] = z;
    return result;
}

// Create a rotation matrix around the X-axis
Matrix matrix_rotate_x(float angle) {
    Matrix result = matrix_identity();
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    result.m[1][1] = cos_angle;
    result.m[1][2] = sin_angle;
    result.m[2][1] = -sin_angle;
    result.m[2][2] = cos_angle;

    return result;
}

// Create a rotation matrix around the Y-axis
Matrix matrix_rotate_y(float angle) {
    Matrix result = matrix_identity();
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    result.m[0][0] = cos_angle;
    result.m[0][2] = -sin_angle;
    result.m[2][0] = sin_angle;
    result.m[2][2] = cos_angle;

    return result;
}

// Create a rotation matrix around the Z-axis
Matrix matrix_rotate_z(float angle) {
    Matrix result = matrix_identity();
    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);

    result.m[0][0] = cos_angle;
    result.m[0][1] = sin_angle;
    result.m[1][0] = -sin_angle;
    result.m[1][1] = cos_angle;

    return result;
}

// Create a scaling matrix
Matrix matrix_scale(float x, float y, float z) {
    Matrix result = matrix_identity();
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    return result;
}

Vec3 transform_vector(Vec3 v, const Matrix m) {
    Vec3 result;
    result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }

    return result;
}

