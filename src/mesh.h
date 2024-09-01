#ifndef MESH_H
#define MESH_H

#include "vecmath.h"

typedef struct {
    Vec3 *vertices;
    Vec2 *texcoords;
    Vec3 *normals;
    int num_vertices;
    int num_texcoords;
    int num_normals;
    struct {
        int v[3];
        int t[3];
        int n[3];
    } *faces;
    int num_faces;
} Mesh;

Mesh load_mesh(const char *filename);
void free_mesh(Mesh *mesh);

#endif

