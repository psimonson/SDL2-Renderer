#include "vecmath.h"
#include <math.h>

// Create a Vec3
Vec3 vec3(float x, float y, float z) {
    Vec3 v = {x, y, z};
    return v;
}

// Add two Vec3
Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Subtract two Vec3
Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Scale a Vec3 by a scalar
Vec3 vec3_scale(Vec3 v, float scale) {
    return vec3(v.x * scale, v.y * scale, v.z * scale);
}

// Normalize a Vec3
Vec3 vec3_normalize(Vec3 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0) {
        return vec3(v.x / length, v.y / length, v.z / length);
    }
    return vec3(0, 0, 0);
}

// Compute the cross product of two Vec3
Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Compute the dot product of two Vec3
float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

