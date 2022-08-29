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

bool Platform::HandleInput(uint8_t* keypad) const {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return false;
                    case SDLK_1:
                        keypad[0x1] = 0;
                        break;
                    case SDLK_2:
                        keypad[0x2] = 0;
                        break;
                    case SDLK_3:
                        keypad[0x3] = 0;
                        break;
                    case SDLK_4:
                        keypad[0xC] = 0;
                        break;
                    case SDLK_q:
                        keypad[0x4] = 0;
                        break;
                    case SDLK_w:
                        keypad[0x5] = 0;
                        break;
                    case SDLK_e:
                        keypad[0x6] = 0;
                        break;
                    case SDLK_r:
                        keypad[0xD] = 0;
                        break;
                    case SDLK_a:
                        keypad[0x7] = 0;
                        break;
                    case SDLK_s:
                        keypad[0x8] = 0;
                        break;
                    case SDLK_d:
                        keypad[0x9] = 0;
                        break;
                    case SDLK_f:
                        keypad[0xE] = 0;
                        break;
                    case SDLK_z:
                        keypad[0xA] = 0;
                        break;
                    case SDLK_x:
                        keypad[0x0] = 0;
                        break;
                    case SDLK_c:
                        keypad[0xB] = 0;
                        break;
                    case SDLK_v:
                        keypad[0xF] = 0;
                        break;
                    }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return false;
                    case SDLK_1:
                        keypad[0x1] = 1;
                        break;
                    case SDLK_2:
                        keypad[0x2] = 1;
                        break;
                    case SDLK_3:
                        keypad[0x3] = 1;
                        break;
                    case SDLK_4:
                        keypad[0xC] = 1;
                        break;
                    case SDLK_q:
                        keypad[0x4] = 1;
                        break;
                    case SDLK_w:
                        keypad[0x5] = 1;
                        break;
                    case SDLK_e:
                        keypad[0x6] = 1;
                        break;
                    case SDLK_r:
                        keypad[0xD] = 1;
                        break;
                    case SDLK_a:
                        keypad[0x7] = 1;
                        break;
                    case SDLK_s:
                        keypad[0x8] = 1;
                        break;
                    case SDLK_d:
                        keypad[0x9] = 1;
                        break;
                    case SDLK_f:
                        keypad[0xE] = 1;
                        break;
                    case SDLK_z:
                        keypad[0xA] = 1;
                        break;
                    case SDLK_x:
                        keypad[0x0] = 1;
                        break;
                    case SDLK_c:
                        keypad[0xB] = 1;
                        break;
                    case SDLK_v:
                        keypad[0xF] = 1;
                        break;
                }
                break;
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