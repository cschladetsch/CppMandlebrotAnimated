#pragma once
#include <string>
#include <GL/glew.h>

GLuint createShaderProgram(const std::string& fragPath);
std::string loadFile(const std::string& path);
void checkShaderCompile(GLuint shader, const std::string& type);
