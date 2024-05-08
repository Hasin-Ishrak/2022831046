#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#define SCREENWIDTH 500
#define SCREENHEIGHT 500

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Task_101", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Circle(SDL_Renderer* renderer, int centerX,int centerY,int radius) {
    for (int a= -radius; a <= radius; a++) {
        for (int b = -radius; b <= radius; b++) {
            if (a*a + b*b <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX+a, centerY+b);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!init(&window, &renderer)) {
        return 1;
    }

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        } 
        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 204,255,255, 255);
        Circle(renderer, SCREENWIDTH / 2, SCREENHEIGHT / 2, 100);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}