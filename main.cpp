#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <iostream>
#include <vector>

// Constantes para la pantalla
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Constantes para el juego
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int BALL_SIZE = 15;
const float BALL_INITIAL_SPEED = 200.0f; // pixels per second
const int BRICK_ROWS = 5;
const int BRICK_COLUMNS = 10;
const int BRICK_WIDTH = 60;
const int BRICK_HEIGHT = 20;

// Estructuras para los componentes del juego
struct Position {
    float x, y;
};

struct Velocity {
    float vx, vy;
};

struct Paddle {
    int width, height;
};

struct Ball {
    float speed;
};

struct Brick {
    int width, height;
};

void handle_input(entt::registry& registry, float deltaTime) {
    auto view = registry.view<Position, Paddle>();

    for (auto entity : view) {
        auto& position = view.get<Position>(entity);
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT]) {
            position.x -= 300 * deltaTime; // 300 pixels per second
            if (position.x < 0) position.x = 0;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            position.x += 300 * deltaTime;
            if (position.x > SCREEN_WIDTH - PADDLE_WIDTH) position.x = SCREEN_WIDTH - PADDLE_WIDTH;
        }
    }
}

void update_ball(entt::registry& registry, float deltaTime) {
    auto view = registry.view<Position, Velocity, Ball>();

    for (auto entity : view) {
        auto& position = view.get<Position>(entity);
        auto& velocity = view.get<Velocity>(entity);
        auto& ball = view.get<Ball>(entity);

        position.x += velocity.vx * deltaTime;
        position.y += velocity.vy * deltaTime;

        // Checar colisiones con las paredes
        if (position.x <= 0 || position.x + BALL_SIZE >= SCREEN_WIDTH) {
            velocity.vx = -velocity.vx;
            ball.speed += 50.0f;
        }
        if (position.y <= 0) {
            velocity.vy = -velocity.vy;
            ball.speed += 50.0f;
        }

        // Colisión con el paddle
        auto paddle_view = registry.view<Position, Paddle>();
        for (auto paddle_entity : paddle_view) {
            auto& paddle_pos = paddle_view.get<Position>(paddle_entity);
            auto& paddle = paddle_view.get<Paddle>(paddle_entity);

            if (position.x < paddle_pos.x + paddle.width && position.x + BALL_SIZE > paddle_pos.x &&
                position.y < paddle_pos.y + paddle.height && position.y + BALL_SIZE > paddle_pos.y) {
                velocity.vy = -velocity.vy;
                ball.speed += 50.0f;
                position.y = paddle_pos.y - BALL_SIZE; // Ajustar la posición para evitar colisiones continuas
            }
        }

        // Colisión con los ladrillos
        auto brick_view = registry.view<Position, Brick>();
        for (auto brick_entity : brick_view) {
            auto& brick_pos = brick_view.get<Position>(brick_entity);
            auto& brick = brick_view.get<Brick>(brick_entity);

            if (position.x < brick_pos.x + brick.width && position.x + BALL_SIZE > brick_pos.x &&
                position.y < brick_pos.y + brick.height && position.y + BALL_SIZE > brick_pos.y) {
                velocity.vy = -velocity.vy;
                registry.destroy(brick_entity);
                ball.speed += 50.0f;
                break; // Solo una colisión por frame
            }
        }

        if (position.y + BALL_SIZE >= SCREEN_HEIGHT) {
            std::cout << "Game Over!" << std::endl;
            SDL_Quit();
            exit(0);
        }
    }
}

void render_game(entt::registry& registry, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear with black color
    SDL_RenderClear(renderer);

    auto view = registry.view<Position, Paddle>();
    for (auto entity : view) {
        auto& position = view.get<Position>(entity);
        auto& paddle = view.get<Paddle>(entity);

        SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y), paddle.width, paddle.height };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White paddle
        SDL_RenderFillRect(renderer, &rect);
    }

    auto ball_view = registry.view<Position, Ball>();
    for (auto entity : ball_view) {
        auto& position = ball_view.get<Position>(entity);

        SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y), BALL_SIZE, BALL_SIZE };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White ball
        SDL_RenderFillRect(renderer, &rect);
    }

    auto brick_view = registry.view<Position, Brick>();
    for (auto entity : brick_view) {
        auto& position = brick_view.get<Position>(entity);
        auto& brick = brick_view.get<Brick>(entity);

        SDL_Rect rect = { static_cast<int>(position.x), static_cast<int>(position.y), brick.width, brick.height };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red bricks
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

void check_win_condition(entt::registry& registry) {
    auto brick_view = registry.view<Brick>();
    if (brick_view.empty()) {
        std::cout << "Ganaste!" << std::endl;
        SDL_Quit();
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    entt::registry registry;

    // Crear Paddle
    auto paddle = registry.create();
    registry.emplace<Position>(paddle, SCREEN_WIDTH / 2.0f - PADDLE_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f);
    registry.emplace<Paddle>(paddle, PADDLE_WIDTH, PADDLE_HEIGHT);

    // Crear Ball
    auto ball = registry.create();
    registry.emplace<Position>(ball, SCREEN_WIDTH / 2.0f - BALL_SIZE / 2.0f, SCREEN_HEIGHT / 2.0f - BALL_SIZE / 2.0f);
    registry.emplace<Velocity>(ball, BALL_INITIAL_SPEED, BALL_INITIAL_SPEED);
    registry.emplace<Ball>(ball, BALL_INITIAL_SPEED);

    // Crear Bricks
    float offsetX = (SCREEN_WIDTH - (BRICK_COLUMNS * BRICK_WIDTH)) / 2.0f;
    float offsetY = 50.0f;

    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLUMNS; ++col) {
            auto brick = registry.create();
            registry.emplace<Position>(brick, offsetX + col * (BRICK_WIDTH + 5), offsetY + row * (BRICK_HEIGHT + 5));
            registry.emplace<Brick>(brick, BRICK_WIDTH, BRICK_HEIGHT);
        }
    }

    bool quit = false;
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Time in seconds
        lastTime = currentTime;

        handle_input(registry, deltaTime);
        update_ball(registry, deltaTime);
        check_win_condition(registry);
        render_game(registry, renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
