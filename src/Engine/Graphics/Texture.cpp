#include "Texture.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream> // Agregar esta línea

Texture::Texture() : texture(nullptr), width(0), height(0) {}

Texture::~Texture() {
    free();
}

void Texture::load(const std::string& path, SDL_Renderer* renderer) {
    free();
    std::cout << "Attempting to load texture from: " << path << std::endl;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (!texture) {
            std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Texture successfully created from: " << path << std::endl;
        }
        width = loadedSurface->w;
        height = loadedSurface->h;
        SDL_FreeSurface(loadedSurface);
    }
}



void Texture::free() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        width = 0;
        height = 0;
    }
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
    SDL_Rect srcrect = {x, y, width, height};
    if (clip != nullptr) {
        srcrect.w = clip->w;
        srcrect.h = clip->h;
    }
    SDL_RenderCopy(renderer, texture, &srcrect, clip);
}
