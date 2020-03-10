#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader();
    Shader(std::string const& vertexPath, std::string const& fragmentPath);
    void init(std::string const& vertexPath, std::string const& fragmentPath);
    void set(std::string const& name, int value) const;
    void set(std::string const& name, float value) const;
    void set(std::string const& name, glm::mat4 const& matrix) const;
    void use() const;

    GLuint ID;
private:
    void checkCompileErrors(GLuint shader, std::string const& type);
};
