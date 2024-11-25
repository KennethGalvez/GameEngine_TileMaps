#pragma once
#include <entt/entt.hpp>
#include "Scene.h"
#include "Components.h"
#include <iostream>

class Entity {
public:
    Entity(entt::entity handle, Scene* scene)
        : handle(handle), scene(scene) {
        std::cout << "Entity Created" << std::endl;
    }

    ~Entity() {
        scene->registry.destroy(handle);
        std::cout << "Entity Destroyed" << std::endl;
    }

    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
    }

private:
    entt::entity handle;
    Scene* scene;
};
