#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"

class Game {
public:
    Game(const char* title, int width, int height);
    virtual ~Game();

    virtual void setup() = 0;
    void handleEvents();
    void update();
    void render();
    void run();
    void setScene(Scene* newScene);
    Scene* getCurrentScene() const;

protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    int screen_width;
    int screen_height;
    Scene* currentScene;

    template<typename T>
    void addRenderSystem(Scene* scene) {
        auto system = new T();
        scene->renderSystems.push_back(system);
    }
};
