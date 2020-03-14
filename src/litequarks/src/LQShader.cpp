#include <litequarks/LQShader.hpp>
#include <glm/mat4x4.hpp>

#include <exception>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

LQShader::LQShader()
: ID(0)
{
}

LQShader::LQShader(std::string const& vertexPath, std::string const& fragmentPath)
: LQShader()
{
    init(vertexPath, fragmentPath);
}

void LQShader::init(std::string const& vertexPath, std::string const& fragmentPath) {
    ID = glCreateProgram();
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);

    if (vShaderFile.is_open() && fShaderFile.is_open()) {
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    else {
        std::cerr << "can not open shader files" << std::endl;
        std::terminate();
    }

    char const* vShaderCode = vertexCode.c_str();
    char const* fShaderCode = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void LQShader::set(std::string const& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void LQShader::set(std::string const& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void LQShader::set(std::string const& name, glm::mat4 const& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

// void setVec2(const std::string &name, const glm::vec2 &value) const
// { 
//     glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
// }
// void setVec2(const std::string &name, float x, float y) const
// { 
//     glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
// }
// // ------------------------------------------------------------------------
// void setVec3(const std::string &name, const glm::vec3 &value) const
// { 
//     glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
// }
// void setVec3(const std::string &name, float x, float y, float z) const
// { 
//     glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
// }
// // ------------------------------------------------------------------------
// void setVec4(const std::string &name, const glm::vec4 &value) const
// { 
//     glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
// }
// void setVec4(const std::string &name, float x, float y, float z, float w) 
// { 
//     glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
// }
// // ------------------------------------------------------------------------
// void setMat2(const std::string &name, const glm::mat2 &mat) const
// {
//     glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
// }
// // ------------------------------------------------------------------------
// void setMat3(const std::string &name, const glm::mat3 &mat) const
// {
//     glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
// }
// // ------------------------------------------------------------------------
// void setMat4(const std::string &name, const glm::mat4 &mat) const
// {
//     glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
// }

void LQShader::use() const {
    glUseProgram(ID);
}

void LQShader::checkCompileErrors(GLuint shader, std::string const& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
                      << type << "\n" << infoLog << std::endl;
            std::terminate();
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR:\n"
                      << infoLog << std::endl;
            std::terminate();
        }
    }
}
