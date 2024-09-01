#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Mesh load_mesh(const char *filename) {
    Mesh mesh = {0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            mesh.num_vertices++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            mesh.num_texcoords++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            mesh.num_normals++;
        } else if (strncmp(line, "f ", 2) == 0) {
            mesh.num_faces++;
        }
    }

    fseek(file, 0, SEEK_SET);
    mesh.vertices = (Vec3 *)malloc(sizeof(Vec3) * mesh.num_vertices);
    mesh.texcoords = (Vec2 *)malloc(sizeof(Vec2) * mesh.num_texcoords);
    mesh.normals = (Vec3 *)malloc(sizeof(Vec3) * mesh.num_normals);
    mesh.faces = (void *)malloc(sizeof(*mesh.faces) * mesh.num_faces);

    int v_idx = 0, vt_idx = 0, vn_idx = 0, f_idx = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            sscanf(line + 2, "%f %f %f", &mesh.vertices[v_idx].x, &mesh.vertices[v_idx].y, &mesh.vertices[v_idx].z);
            v_idx++;
        } else if (strncmp(line, "vt ", 3) == 0) {
            sscanf(line + 3, "%f %f", &mesh.texcoords[vt_idx].x, &mesh.texcoords[vt_idx].y);
            vt_idx++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            sscanf(line + 3, "%f %f %f", &mesh.normals[vn_idx].x, &mesh.normals[vn_idx].y, &mesh.normals[vn_idx].z);
            vn_idx++;
        } else if (strncmp(line, "f ", 2) == 0) {
            sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                &mesh.faces[f_idx].v[0], &mesh.faces[f_idx].t[0], &mesh.faces[f_idx].n[0],
                &mesh.faces[f_idx].v[1], &mesh.faces[f_idx].t[1], &mesh.faces[f_idx].n[1],
                &mesh.faces[f_idx].v[2], &mesh.faces[f_idx].t[2], &mesh.faces[f_idx].n[2]);
            f_idx++;
        }
    }

    fclose(file);
    return mesh;
}

void free_mesh(Mesh *mesh) {
    free(mesh->vertices);
    free(mesh->texcoords);
    free(mesh->normals);
    free(mesh->faces);
}

