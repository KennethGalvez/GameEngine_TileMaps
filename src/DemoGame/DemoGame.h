#pragma once
#include "Game.h"
#include "Scene.h"
#include "Systems.h"
#include <iostream>
#include <entt/entt.hpp>

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

        // textura de fondo
        SDL_Texture* backgroundTexture = loadTexture("src/DemoGame/Fondo.png", renderer);

        // Verificar la textura
        if (!backgroundTexture) {
            std::cerr << "Failed to load background texture." << std::endl;
            return;
        }

        // Sistema de renderizado de fondo y agregarlo manualmente a renderSystems
        BackgroundRenderSystem* backgroundSystem = new BackgroundRenderSystem(backgroundTexture);
        sampleScene->renderSystems.push_back(backgroundSystem);

        
        //auto squareSystem = new SquareRenderSystem();
        //sampleScene->renderSystems.push_back(squareSystem);

        setScene(sampleScene);
    }



};
