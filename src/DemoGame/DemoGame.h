#pragma once
#include "Game.h"
#include "Scene.h"
#include "Systems.h"
#include <iostream>
#include <entt/entt.hpp>
#include "Entity.h"


class SquareRenderSystem : public RenderSystem {
  public:
    void run(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_Rect r = { 10, 10, 100, 100 };
        SDL_RenderFillRect(renderer, &r);
    }
}; 

class DemoGame : public Game {
  public:
    Scene* sampleScene;
    entt::registry registry;

    DemoGame()
        : Game("SAMPLE GAME", 1024, 768) {}

    void setup() override {
        std::cout << "HELLO WORLD" << std::endl;  
        sampleScene = new Scene("SAMPLE SCENE", registry);

        // Fondo
        SDL_Texture* backgroundTexture = loadTexture("src/DemoGame/Fondo.png", renderer);
        sampleScene->renderSystems.push_back(new BackgroundRenderSystem(backgroundTexture));

        // Crear personaje
        auto* character = sampleScene->createEntity("Player");
        character->addComponent<PositionComponent>(400, 300); // Coordenadas iniciales
        character->addComponent<DirectionComponent>(DirectionComponent::RIGHT);

        SDL_Texture* idleFrame1 = loadTexture("src/DemoGame/Personaje.png", renderer);
        if (!idleFrame1) {
            std::cerr << "Error: No se pudo cargar Personaje.png" << std::endl;
        }
        SDL_Texture* idleFrame2 = loadTexture("src/DemoGame/Personaje3.png", renderer);
        if (!idleFrame2) {
            std::cerr << "Error: No se pudo cargar Personaje3.png" << std::endl;
        }

        character->addComponent<TextureComponent>(idleFrame1, 150, 100); // Tamaño del sprite
        character->addComponent<AnimationComponent>(std::vector<SDL_Texture*>{idleFrame1, idleFrame2}, 0, 0.1f, 0.0f);

        // Agregar sistemas
        sampleScene->renderSystems.push_back(new CharacterRenderSystem());
        sampleScene->inputSystems.push_back(new InputSystem());

        setScene(sampleScene);
    }


};
