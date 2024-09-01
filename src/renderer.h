#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "mesh.h"
#include "vecmath.h"
#include "matrix.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int width, height;
} Renderer;

Renderer create_renderer(SDL_Window *window, int width, int height);
void destroy_renderer(Renderer *renderer);

void draw_pixel(Renderer *renderer, Vec3 v, SDL_Color color);
void draw_line(Renderer *renderer, Vec3 v1, Vec3 v2, SDL_Color color);
void draw_triangle(Renderer *renderer, Matrix mvp, Vec3 v1, Vec3 v2, Vec3 v3, SDL_Color color);
void draw_textured_triangle(Renderer *renderer, Vec3 v1, Vec3 v2, Vec3 v3, Vec2 t1, Vec2 t2, Vec2 t3);
void draw_quad(Renderer *renderer, Matrix mvp, Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4, SDL_Color color);
void draw_textured_quad(Renderer *renderer, Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4, Vec2 t1, Vec2 t2, Vec2 t3, Vec2 t4);
void render_mesh(Renderer *renderer, Matrix mvp, Mesh *mesh, SDL_Color color);

#endif

