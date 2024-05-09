#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define ScreenWidth 800
#define ScreenHeight 800
#define autoCircleRadius 50
#define autoCircleSpeed 5
#define controlCircleRadius 30

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Initialization Failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Task_103", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window Failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer  Failed: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void Circle(SDL_Renderer* renderer, int center_X, int center_Y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int a = -radius; a <= radius; a++) {
        for (int b = -radius; b <= radius; b++) {
            if (a * a + b * b <= radius * radius) {
                SDL_RenderDrawPoint(renderer, center_X + a, center_Y + b);
            }
        }
    }
}

bool Collision(int x1, int y1, int x2, int y2, int radius1, int radius2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distanceSquared = dx * dx + dy * dy;
    int radiusSumSquared = (radius1 + radius2) * (radius1 + radius2);
    return distanceSquared <= radiusSumSquared;
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!init(&window, &renderer)) {
        return 1;
    }

    SDL_Event event;
    bool running = true;
    int CircleX = 0;
    int CircleY = ScreenHeight / 2;
    int cCircleX = ScreenWidth / 2;
    int cCircleY = 20;
    SDL_Color CircleColor = {255, 255, 51, 255}; 
    SDL_Color controlCircleColor = {135, 206, 250, 255};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP) {
                    cCircleY -= 10;
                } 
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    cCircleY += 10;
                } 
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    cCircleX -= 10;
                } 
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    cCircleX += 10;
                }
            }
        }

        CircleX += autoCircleSpeed;
        if (CircleX - autoCircleRadius > ScreenWidth) {
            CircleX = -autoCircleRadius;
        }

        if (Collision(CircleX, CircleY, cCircleX, cCircleY, autoCircleRadius, controlCircleRadius)) {
            CircleColor = controlCircleColor = {255, 0, 0, 255}; 
        } else {
            CircleColor = {255, 255, 51, 255}; 
            controlCircleColor = {135, 206, 250, 255}; 
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderClear(renderer);

        Circle(renderer, CircleX, CircleY, autoCircleRadius, CircleColor);
        Circle(renderer, cCircleX, cCircleY, controlCircleRadius, controlCircleColor);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
