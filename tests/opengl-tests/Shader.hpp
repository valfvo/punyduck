#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader(std::string const& vertexPath, std::string const& fragmentPath);
    void set(std::string const& name, int value) const;
    void set(std::string const& name, float value) const;
    void use() const;

    GLuint ID;
private:
    void checkCompileErrors(GLuint shader, std::string const& type);
};
