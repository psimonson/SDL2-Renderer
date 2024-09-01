#ifndef MATRIX_H
#define MATRIX_H

#include "vecmath.h"

typedef struct {
    float m[4][4];
} Matrix;

// Function declarations
Matrix matrix_identity();
Matrix matrix_multiply(const Matrix *a, const Matrix *b);

Matrix set_perspective_projection(float fov, float aspect, float near, float far);
Matrix set_orthographic_projection(float left, float right, float bottom, float top, float near, float far);
Matrix matrix_look_at(Vec3 eye, Vec3 target, Vec3 up);

Matrix matrix_translate(float x, float y, float z);
Matrix matrix_rotate_x(float angle);
Matrix matrix_rotate_y(float angle);
Matrix matrix_rotate_z(float angle);
Matrix matrix_scale(float x, float y, float z);
Vec3 transform_vector(Vec3 v, const Matrix m);

#endif // MATRIX_H

