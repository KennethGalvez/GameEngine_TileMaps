#include "Scene.h"
#include "Entity.h"
#include <iostream>
#include <SDL2/SDL_image.h>

Scene::Scene(const std::string& name, entt::registry& registry)
    : name(name), registry(registry) {
    std::cout << "Scene Initialized: " << name << std::endl;
}

Scene::~Scene() {
    std::cout << "Scene Destroyed: " << name << std::endl;
}

Entity* Scene::createEntity(const std::string& name) {
    entt::entity entity = registry.create();
    Entity* newEntity = new Entity(entity, this);
    newEntity->addComponent<NameComponent>(name);
    return newEntity;
}

void Scene::setup() {
    // Setup logic here
}

void Scene::render(SDL_Renderer* renderer) {
    for (auto& system : renderSystems) {
        // Detectar si es CharacterRenderSystem
        if (auto* charSystem = dynamic_cast<CharacterRenderSystem*>(system)) {
            charSystem->run(renderer, registry, 0.016f); // Aproximadamente 60 FPS
        } else {
            system->run(renderer);
        }
    }
}


// Implementation of processEvents
void Scene::processEvents(SDL_Event& event) {
    for (auto& system : inputSystems) {
        // Procesar eventos en sistemas de input
        dynamic_cast<InputSystem*>(system)->processEvents(event, registry);
    }
}


// Implementation of update
void Scene::update(float deltaTime) {
    for (auto& system : renderSystems) {
        if (auto* charSystem = dynamic_cast<CharacterRenderSystem*>(system)) {
            charSystem->run(nullptr, registry, deltaTime); // Actualizar animación
        }
    }
}


// Función para cargar una textura desde un archivo
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Error imagen: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
