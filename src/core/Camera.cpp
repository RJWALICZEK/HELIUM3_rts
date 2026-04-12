#include "Camera.h"

void Camera::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

float Camera::screenToWorldX(int screenX) const {
    return screenX + x;
}
float Camera::screenToWorldY(int screenY) const {
    return screenY + y;
}

void Camera::update(float deltaTime) {

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    float speed = 450.0f * deltaTime; //camera speed

    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) x -= speed;
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) x += speed;
    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) y -= speed;
    if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) y += speed;

    //edge scrolling
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    const int edge = 25; //cursor distance from camera edge

    if (my < edge) {
        y -= speed * 1.3f;
    }
    if (my > 600 - edge) {
        y += speed * 1.3f;
    }
    if (mx < edge) {
        x -= speed * 1.3f;
    }
    if (mx > 800 - edge) {
        x += speed * 1.3f;
    }

    const int WORLD_WIDTH = 2400;
    const int WORLD_HEIGHT = 1800;


    //camera limit to map borders
    if (x < 0.0f) x = 0.0f;
    if (y < 0.0f) y = 0.0f;
    if (x > WORLD_WIDTH - 800) x = WORLD_WIDTH - 800;
    if (y > WORLD_HEIGHT - 600) y = WORLD_HEIGHT - 600;
}