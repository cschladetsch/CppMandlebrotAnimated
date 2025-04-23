#pragma once
#include "config.hpp"
#include <glm/glm.hpp>

class MandelbrotApp {
public:
    explicit MandelbrotApp(const AppConfig& config);
    int run();

private:
    AppConfig config;
    bool isDragging;
    glm::vec2 lastMousePos;
};
