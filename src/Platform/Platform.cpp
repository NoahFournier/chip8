#include "Platform.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>

Platform::Platform(const std::string &title, unsigned int windowWidth, unsigned int windowHeight, unsigned int videoPitch)
    : videoPitch{videoPitch} {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR("SDL_Init: ", SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth * 10, windowHeight * 10, SDL_WINDOW_SHOWN);
    if (!window) {
        ERROR("SDL_CreateWindow: ", SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        ERROR("SDL_CreateRenderer: ", SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
                                windowWidth, windowHeight);
    if (!texture) {
        ERROR("SDLCreateTexture: ", SDL_GetError());
        throw std::runtime_error(SDL_GetError());
    }
}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Platform::HandleInput() const {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
        }
    }
    return true;
}

void Platform::Draw(const uint32_t *display) {
    SDL_UpdateTexture(texture, nullptr, display, videoPitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}