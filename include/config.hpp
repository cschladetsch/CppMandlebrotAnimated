#pragma once
#include <glm/glm.hpp>

struct AppConfig {
    int width = 400;
    int height = 200;
    float scale = 2.5f;
    glm::vec2 center = {-0.75f, 0.0f};
};

AppConfig parseCommandLine(int argc, char** argv);
