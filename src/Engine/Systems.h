#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"
#include "Components.h"

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

class InputSystem : public System {
public:
    void run(SDL_Renderer* renderer) override {
        // Logica
    }

    void processEvents(SDL_Event& event, entt::registry& registry) {
        auto view = registry.view<PositionComponent, DirectionComponent>();

        for (auto entity : view) {
            auto& position = view.get<PositionComponent>(entity);
            auto& direction = view.get<DirectionComponent>(entity);

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    position.x -= 5; // Mover a la izquierda
                    direction.direction = DirectionComponent::LEFT;
                    break;
                case SDLK_RIGHT:
                    position.x += 5; // Mover a la derecha
                    direction.direction = DirectionComponent::RIGHT;
                    break;
                }
            }
        }
    }
};

class CharacterRenderSystem : public RenderSystem {
public:
    // Implementación del método virtual puro
    void run(SDL_Renderer* renderer) override {
        // método vacío
    }

    // Método principal que utiliza el sistema
    void run(SDL_Renderer* renderer, entt::registry& registry, float deltaTime) {
        auto view = registry.view<PositionComponent, TextureComponent, AnimationComponent, DirectionComponent>();

        for (auto entity : view) {
            auto& position = view.get<PositionComponent>(entity);
            auto& texture = view.get<TextureComponent>(entity);
            auto& animation = view.get<AnimationComponent>(entity);
            auto& direction = view.get<DirectionComponent>(entity);

            // Actualizar animación
            animation.elapsedTime += deltaTime;
            if (animation.elapsedTime >= animation.frameTime) {
                animation.currentFrame = (animation.currentFrame + 1) % animation.frames.size();
                animation.elapsedTime = 0.0f;
            }

            SDL_Texture* currentTexture = animation.frames[animation.currentFrame];
            SDL_Rect dstRect = { position.x, position.y, texture.width, texture.height };

            // Voltear sprite si se mueve hacia la izquierda
            SDL_RendererFlip flip = (direction.direction == DirectionComponent::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            SDL_RenderCopyEx(renderer, currentTexture, nullptr, &dstRect, 0, nullptr, flip);
        }
    }
};
