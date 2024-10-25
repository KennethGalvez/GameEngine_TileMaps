#pragma once
#include <string>
#include <SDL2/SDL.h>

struct NameComponent {
    std::string name;
};

struct PositionComponent {
    int x, y;
};

struct TextureComponent {
    SDL_Texture* texture;
    int width;
    int height;
};