#include <iostream>
#include <chrono>
#include <iterator>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_timer.h"

int main (int argc, char *argv[]) {
    std::cout << "hello world!" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cout << "Failed to initialize the SDL2 image library\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("My first window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/tile000.png");
    int xPos = 0;
    int yPos = 100;
    int xVel = 0;
    int yVel = 0;
    int xDir = 0;
    int yDir = 0;
    const int DEFAULT_VEL = 5;
    const int TICK_RATE = 16;

    bool running = true;
    SDL_Event e;
    auto previousTick = SDL_GetPerformanceCounter();
    double lag = 0.0;
    while (running) {
        auto currentTick = SDL_GetPerformanceCounter();
        double delta = (double) (currentTick - previousTick) / SDL_GetPerformanceFrequency();  
        double deltaMs = delta * 1000;
        previousTick = currentTick;
        lag += deltaMs;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                double vel = DEFAULT_VEL;
                switch (e.key.keysym.sym) {
                    case SDLK_a: xDir = -1; break;
                    case SDLK_d: xDir = 1; break;
                    case SDLK_w: yDir = -1; break;
                    case SDLK_s: yDir = 1; break;
                }
            } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_a: xDir = 0; break;
                    case SDLK_d: xDir = 0; break;
                    case SDLK_w: yDir = 0; break;
                    case SDLK_s: yDir = 0; break;
                }
            }
        }

        xVel = DEFAULT_VEL * xDir;
        yVel = DEFAULT_VEL * yDir;

        int updateCounter = 0;
        while (lag >= TICK_RATE) {
            if (updateCounter > 10) {
                break;
            }
            xPos += xVel;
            yPos += yVel;

            lag -= TICK_RATE;
            updateCounter++;
        }

        double renderRatio = lag / TICK_RATE;
        SDL_RenderClear(renderer);
        SDL_Rect dstrect = { (int) (xPos + (xVel * renderRatio)), (int) (yPos + (yVel * renderRatio)), 192, 192};
        //SDL_Rect dstrect = { xPos, yPos, 192, 192};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
