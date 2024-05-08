#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 800
#define CIRCLERADIUS 50
#define CIRCLESPEED 5
#define DIRECTIONCIRCLERADIUS 30

bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Moving and collision of two circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);
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

void Circle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
     for (int a= -radius; a <= radius; a++) {
        for (int b = -radius; b <= radius; b++) {
            if (a*a + b*b <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX + a, centerY + b);
            }
        }
    }
}

void directionCircle(SDL_Renderer* renderer, int x, int y) {
    Circle(renderer, x, y, DIRECTIONCIRCLERADIUS);
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

    int X = -CIRCLERADIUS;
    int Y = SCREENHEIGHT / 2;
    int directionCircleX = SCREENWIDTH / 2;
    int directionCircleY = 0;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {

                if (event.key.keysym.sym == SDLK_UP) {
                    directionCircleY -= 5;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    directionCircleY += 5;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    directionCircleX -= 5;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    directionCircleX += 5;
                }
            }
        }

      
        X += CIRCLESPEED;
        if (X > SCREENWIDTH + CIRCLERADIUS) {
            X = -CIRCLERADIUS;
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        SDL_RenderClear(renderer);

      
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        Circle(renderer, X, Y, CIRCLERADIUS);

        SDL_SetRenderDrawColor(renderer,  51, 102, 255, 255);
        directionCircle(renderer, directionCircleX, directionCircleY);

       
        if (Collision(X, Y, directionCircleX, directionCircleY, CIRCLERADIUS, DIRECTIONCIRCLERADIUS)) {
           
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, X - CIRCLERADIUS, Y, X + CIRCLERADIUS, Y);
        }

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
