#pragma once
#include <SDL2/SDL.h>

class Camera {
private:
    float x = 0.0f;
    float y = 0.0f;

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
public:
    void update(float deltaTime);
    float getX() const { return x; }
    float getY() const { return y; }

    void setPosition(float newX, float newY);

    float screenToWorldX(int screenX) const;
    float screenToWorldY(int screenY) const;


};




// from game.h
/*void updateCamera();
    void handleCameraMovement();
    float getWorldMouseX(int screenX) const { return screenX + cameraX; }
    float getWorldMouseY(int screenY) const { return screenY + cameraY; }*/