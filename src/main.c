#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include "mesh.h"

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Renderer renderer = create_renderer(window, 800, 600);

	Matrix model = matrix_identity();
	Matrix view = matrix_look_at((Vec3){0.0f, 0.0f, 3.0f}, (Vec3){0.0f, 0.0f, 0.0f}, (Vec3){0.0f, 1.0f, 0.0f});
	Matrix projection = set_perspective_projection(90.0f, (float)renderer.width / (float)renderer.height, 0.1f, 100.0f);
	Matrix projection_view = matrix_multiply(&projection, &view);
	Matrix mvp = matrix_multiply(&model, &projection_view);

    Mesh mesh = load_mesh("gfx/model.obj");

    // Game loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer.renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer.renderer);

        render_mesh(&renderer, mvp, &mesh, (SDL_Color){255, 255, 0, 255});

        SDL_RenderPresent(renderer.renderer);
        SDL_Delay(16); // Roughly 60 FPS
    }

    free_mesh(&mesh);
    destroy_renderer(&renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

