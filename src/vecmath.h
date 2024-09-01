#ifndef VECMATH_H
#define VECMATH_H

typedef struct {
	float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

// Function declarations
Vec3 vec3(float x, float y, float z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, float scale);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_dot(Vec3 a, Vec3 b);

#endif // VEC3_H

