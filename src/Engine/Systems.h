#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"

class System {
public:
    virtual void run(SDL_Renderer* renderer) = 0;
};

class RenderSystem : public System {
public:
    virtual void run(SDL_Renderer* renderer) override = 0;
};

class BackgroundRenderSystem : public RenderSystem {
public:
    BackgroundRenderSystem(SDL_Texture* backgroundTexture)
        : texture(backgroundTexture) {}

    void run(SDL_Renderer* renderer) override {
        if (texture) {
            SDL_Rect dstRect = {0, 0, 1024, 768}; // Ajustar al tamaño de la ventana
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        }
    }

private:
    SDL_Texture* texture;
};
