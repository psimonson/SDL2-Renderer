#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "vecmath.h"
#include "matrix.h"
#include "renderer.h"

Renderer create_renderer(SDL_Window *window, int width, int height) {
    Renderer renderer = {0};
    renderer.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    renderer.width = width;
    renderer.height = height;
    if (!renderer.renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    return renderer;
}

void destroy_renderer(Renderer *renderer) {
    SDL_DestroyRenderer(renderer->renderer);
}

Vec3 ndc_to_screen(Vec3 v, int screen_width, int screen_height) {
    return vec3(
        (v.x + 1.0f) * 0.5f * screen_width,
        (1.0f - v.y) * 0.5f * screen_height, // Note: Flipping y for screen space
        v.z
    );
}

void draw_pixel(Renderer *renderer, Vec3 v, SDL_Color color) {
    Vec3 screen_pos = ndc_to_screen(v, renderer->width, renderer->height);
    SDL_SetRenderDrawColor(renderer->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer->renderer, screen_pos.x, screen_pos.y);
}

void draw_line(Renderer *renderer, Vec3 v1, Vec3 v2, SDL_Color color) {
    Vec3 p1 = ndc_to_screen(v1, renderer->width, renderer->height);
    Vec3 p2 = ndc_to_screen(v2, renderer->width, renderer->height);
    SDL_SetRenderDrawColor(renderer->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer->renderer, p1.x, p1.y, p2.x, p2.y);
}

void draw_filled_triangle(Renderer *renderer, Matrix mvp, Vec3 v1, Vec3 v2, Vec3 v3, SDL_Color color) {
    // Transform the vertices using the model-view-projection matrix
    Vec3 p0 = transform_vector(v1, mvp);
    Vec3 p1 = transform_vector(v2, mvp);
    Vec3 p2 = transform_vector(v3, mvp);

    // Sort the vertices by y-coordinate (p0.y <= p1.y <= p2.y)
    if (p0.y > p1.y) { Vec3 temp = p0; p0 = p1; p1 = temp; }
    if (p1.y > p2.y) { Vec3 temp = p1; p1 = p2; p2 = temp; }
    if (p0.y > p1.y) { Vec3 temp = p0; p0 = p1; p1 = temp; }

    // Calculate inverse slopes
    float inv_slope_1 = 0, inv_slope_2 = 0;
    if (p1.y - p0.y > 0) inv_slope_1 = (float)(p1.x - p0.x) / (p1.y - p0.y);
    if (p2.y - p0.y > 0) inv_slope_2 = (float)(p2.x - p0.x) / (p2.y - p0.y);

    // Draw upper triangle
    if (p1.y != p0.y) {
        for (int y = p0.y; y <= p1.y; y++) {
            int x_start = p0.x + (y - p0.y) * inv_slope_1;
            int x_end = p0.x + (y - p0.y) * inv_slope_2;
            if (x_start > x_end) { int temp = x_start; x_start = x_end; x_end = temp; }
            for (int x = x_start; x <= x_end; x++) {
                draw_pixel(renderer, vec3(x, y, 0), color);
            }
        }
    }

    // Calculate inverse slopes for the lower triangle
    if (p2.y - p1.y > 0) inv_slope_1 = (float)(p2.x - p1.x) / (p2.y - p1.y);

    // Draw lower triangle
    if (p2.y != p1.y) {
        for (int y = p1.y; y <= p2.y; y++) {
            int x_start = p1.x + (y - p1.y) * inv_slope_1;
            int x_end = p0.x + (y - p0.y) * inv_slope_2;
            if (x_start > x_end) { int temp = x_start; x_start = x_end; x_end = temp; }
            for (int x = x_start; x <= x_end; x++) {
                draw_pixel(renderer, vec3(x, y, 0), color);
            }
        }
    }
}

void draw_triangle(Renderer *renderer, Matrix mvp, Vec3 v1, Vec3 v2, Vec3 v3, SDL_Color color) {
    // Transform the vertices using the model-view-projection matrix
    Vec3 t1 = transform_vector(v1, mvp);
    Vec3 t2 = transform_vector(v2, mvp);
    Vec3 t3 = transform_vector(v3, mvp);

	// Draw the transformed triangle
    draw_line(renderer, t1, t2, color);
    draw_line(renderer, t2, t3, color);
    draw_line(renderer, t3, t1, color);
}

void draw_textured_triangle(Renderer *renderer, Vec3 v1, Vec3 v2, Vec3 v3, Vec2 t1, Vec2 t2, Vec2 t3) {
    // Implement texture mapping here
}

void draw_quad(Renderer *renderer, Matrix mvp, Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4, SDL_Color color) {
    draw_triangle(renderer, mvp, v1, v2, v3, color);
    draw_triangle(renderer, mvp, v1, v3, v4, color);
}

void draw_textured_quad(Renderer *renderer, Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4, Vec2 t1, Vec2 t2, Vec2 t3, Vec2 t4) {
    // Implement textured quad rendering here
}

void render_mesh(Renderer *renderer, Matrix mvp, Mesh *mesh, SDL_Color color) {
    // Loop through faces and render them
    for (int i = 0; i < mesh->num_faces; i++) {
        // Backface culling
        Vec3 v0 = mesh->vertices[mesh->faces[i].v[0] - 1];
        Vec3 v1 = mesh->vertices[mesh->faces[i].v[1] - 1];
        Vec3 v2 = mesh->vertices[mesh->faces[i].v[2] - 1];

        // Transform the vertices using the model-view-projection matrix
        Vec3 t0 = transform_vector(v0, mvp);
        Vec3 t1 = transform_vector(v1, mvp);
        Vec3 t2 = transform_vector(v2, mvp);

        Vec3 normal = vec3_cross(vec3_sub(t1, t0), vec3_sub(t2, t0));
        if (vec3_dot(normal, vec3(0, 0, 1)) < 0) {
            draw_triangle(renderer, mvp, t0, t1, t2, color);
        }
    }
}

