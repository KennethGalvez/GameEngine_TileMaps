#pragma once
#include <string>
#include <SDL2/SDL.h>

struct NameComponent {
    std::string name;
};

struct PositionComponent {
    int x, y;

    PositionComponent(int x = 0, int y = 0) : x(x), y(y) {}
};

struct DirectionComponent {
    enum Direction { LEFT, RIGHT, UP, DOWN };
    Direction direction;

    DirectionComponent(Direction direction = RIGHT) : direction(direction) {}
};

struct TextureComponent {
    SDL_Texture* texture;
    int width, height;

    TextureComponent(SDL_Texture* texture = nullptr, int width = 0, int height = 0)
        : texture(texture), width(width), height(height) {}
};

struct AnimationComponent {
    std::vector<SDL_Texture*> frames;
    int currentFrame;
    float frameTime;
    float elapsedTime;

    AnimationComponent(const std::vector<SDL_Texture*>& frames,
                       int currentFrame = 0,
                       float frameTime = 0.1f,
                       float elapsedTime = 0.0f)
        : frames(frames), currentFrame(currentFrame), frameTime(frameTime), elapsedTime(elapsedTime) {}
};

struct FireComponent {
    bool active;
    float timeRemaining; 

    FireComponent(bool active = false, float timeRemaining = 0.0f)
        : active(active), timeRemaining(timeRemaining) {}
};


