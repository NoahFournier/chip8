//
// Created by Noah Fournier on 23/08/2022.
//

#ifndef CHIP8_PLATFORM_H
#define CHIP8_PLATFORM_H

#include <SDL2/SDL.h>
#include <string>

class Platform {
public:
    Platform(const std::string &title, unsigned int windowWidth, unsigned int windowHeight, unsigned int videoPitch);

    ~Platform();

    unsigned int videoPitch;

    bool HandleInput(uint8_t *keypad) const;

    void Draw(const uint32_t *display);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

};


#endif //CHIP8_PLATFORM_H
