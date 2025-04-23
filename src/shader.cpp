#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << path << std::endl;
        return "";
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

void checkShaderCompile(GLuint shader, const std::string& type) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        std::cerr << type << " shader error: " << buffer << std::endl;
    }
}

GLuint createShaderProgram(const std::string& fragPath) {
    const char* vs = R"(#version 330 core
        out vec2 fragCoord;
        void main() {
            fragCoord = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
            gl_Position = vec4(fragCoord * 2.0 - 1.0, 0.0, 1.0);
        })";

    std::string fsSource = loadFile(fragPath);
    if (fsSource.empty()) {
        std::cerr << "ERROR: Failed to load fragment shader from: " << fragPath << std::endl;
        return 0;
    }

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vs, nullptr);
    glCompileShader(vert);
    checkShaderCompile(vert, "Vertex");

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = fsSource.c_str();
    glShaderSource(frag, 1, &fs, nullptr);
    glCompileShader(frag);
    checkShaderCompile(frag, "Fragment");

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}
