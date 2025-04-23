#include "app.h"
#include "shader.hpp"
#include "config.hpp"

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdio>
#include <cmath>

// Helper function to convert screen coordinates to complex plane coordinates
static glm::vec2 screenToComplex(int x, int y, const glm::vec2& center, float scale, int width, int height) {
    glm::vec2 pixelPos(x, y);
    glm::vec2 uv = (pixelPos - glm::vec2(width / 2.0f, height / 2.0f)) / float(height);
    return center + uv * scale;
}

// Screenshot function
static void saveScreenshot(int w, int h, const std::string& file) {
    std::vector<unsigned char> pixels(w * h * 3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    FILE* f = fopen(file.c_str(), "wb");
    if (!f) return;

    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int y = h - 1; y >= 0; --y)
        fwrite(&pixels[y * w * 3], 1, w * 3, f);
    fclose(f);
    std::cout << "Screenshot saved: " << file << std::endl;
}

MandelbrotApp::MandelbrotApp(const AppConfig& config) : config(config), isDragging(false) {}

int MandelbrotApp::run() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Mandelbrot GPU Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config.width, config.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GL_CreateContext(win);
    glewInit();

    GLuint program = createShaderProgram("shaders/mandelbrot.frag");
    glm::vec2 center = config.center;
    float scale = config.scale;
    float zoomFactor = 0.5f; // Default zoom factor on click (zooms in by 2x)

    auto start = std::chrono::steady_clock::now();
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_p)
                    saveScreenshot(config.width, config.height, "screenshot.ppm");
                else if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                // Add 'i' key to increase maximum iterations for more detail
                else if (e.key.keysym.sym == SDLK_i) {
                    // This will be handled by the shader
                    std::cout << "Increased detail level\n";
                }
            }

            else if (e.type == SDL_MOUSEWHEEL) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);

                glm::vec2 mousePosInComplex = screenToComplex(mx, my, center, scale, config.width, config.height);
                float wheelZoomFactor = (e.wheel.y > 0) ? 0.8f : 1.25f;
                scale *= wheelZoomFactor;
                center = mousePosInComplex + (center - mousePosInComplex) * wheelZoomFactor;
                
                // Print current position for reference when finding interesting spots
                std::cout << "Zoomed to: (" << center.x << ", " << center.y 
                          << ") at scale: " << scale << std::endl;
            }
            
            // Handle mouse button events for drag and zoom
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x;
                    int my = e.button.y;
                    
                    // Convert mouse position to complex plane coordinates
                    glm::vec2 clickPosInComplex = screenToComplex(mx, my, center, scale, config.width, config.height);
                    
                    // Zoom in centered on the clicked point
                    scale *= zoomFactor;
                    center = clickPosInComplex;
                    
                    std::cout << "Zoomed to: (" << center.x << ", " << center.y 
                              << ") at scale: " << scale << std::endl;
                }
                // Right click to start dragging
                else if (e.button.button == SDL_BUTTON_RIGHT) {
                    isDragging = true;
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    lastMousePos = glm::vec2(mx, my);
                }
            }
            // Handle mouse button release
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    isDragging = false;
                }
            }
            // Handle mouse motion for dragging
            else if (e.type == SDL_MOUSEMOTION && isDragging) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                glm::vec2 currentMousePos(mx, my);
                
                // Calculate the difference in pixels
                glm::vec2 delta = currentMousePos - lastMousePos;
                
                // Convert pixel difference to complex plane coordinates (scale by height)
                glm::vec2 deltaComplex = delta * (scale / config.height);
                
                // Invert Y-axis (moving mouse down should move fractal up)
                deltaComplex.y = -deltaComplex.y;
                
                // Update the center position (subtract because we're moving the view window)
                center -= deltaComplex;
                
                // Update last position for next frame
                lastMousePos = currentMousePos;
            }
        }

        float time = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - start).count();

        glViewport(0, 0, config.width, config.height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform2f(glGetUniformLocation(program, "resolution"), config.width, config.height);
        glUniform1f(glGetUniformLocation(program, "time"), time);
        glUniform2f(glGetUniformLocation(program, "center"), center.x, center.y);
        glUniform1f(glGetUniformLocation(program, "scale"), scale);

        // Calculate maximum iterations based on zoom level for better detail
        // More iterations are needed for deeper zooms
        int adaptiveIterations = static_cast<int>(std::min(1000.0f, std::log(1.0f / scale) * 200.0f));
        if (adaptiveIterations < 100) adaptiveIterations = 100;
        
        glUniform1i(glGetUniformLocation(program, "max_iter"), adaptiveIterations);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(win);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
