#pragma once
#include <stdint.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader {
    uint32_t id;
    std::unordered_map<std::string, int32_t> uniformLocationCache;
public:
    void init(const std::string& fileName);
    void destruct();
    inline void bind();
    inline void unbind();
    inline void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    inline void setUniform3f(const std::string& name, float v0, float v1, float v2);
    inline void setUniform1i(const std::string& name, int32_t v);
    inline void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
    inline int32_t getUniformLocation(const std::string& name);
    static int compileShader(uint32_t type, const std::string& source);
    static int createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    static int createShaderProgramFromFile(const std::string& filepath);
};



//Public
void Shader::init(const std::string& fileName) {
    id = createShaderProgramFromFile(fileName);
    if (id == -1)
        std::cout << "Error: shader not loaded from path " << fileName << std::endl;
}
void Shader::destruct() {
    glDeleteProgram(id);
}
inline void Shader::bind() {
    glUseProgram(id);
}
inline void Shader::unbind() {
    glUseProgram(0);
}
inline void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}
inline void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}
inline void Shader::setUniform1i(const std::string& name, int32_t v) {
    glUniform1i(getUniformLocation(name), v);
}
inline void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}



//Private
inline int32_t Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];
    auto retValue = glGetUniformLocation(id, name.c_str());
    if (retValue == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist." << std::endl;
    uniformLocationCache[name] = retValue;
    return retValue;
}
int Shader::compileShader(uint32_t type, const std::string& source) {
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();
    const int srcLen = source.size();
    glShaderSource(id, 1, &src, &srcLen);
    glCompileShader(id);
    //TODO: error handling
    int result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::string shaderTypeString;
        switch (type) {
        case GL_VERTEX_SHADER:
            shaderTypeString = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderTypeString = "fragment";
            break;
        }
        std::cout << "Shader of type " << shaderTypeString << " Has failed to compile" << std::endl;
        std::cout << message << std::endl;
        std::cout << std::endl << std::endl << source << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
int Shader::createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    uint32_t program = glCreateProgram();
    uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
int Shader::createShaderProgramFromFile(const std::string& filepath) {
    enum class SHADER_TYPE {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::fstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    SHADER_TYPE type = SHADER_TYPE::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader ") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = SHADER_TYPE::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = SHADER_TYPE::FRAGMENT;
            }
            else {
                std::cout << "Error: unkown shader type!" << std::endl;
                return -1;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return Shader::createShaderProgram(ss[(int)SHADER_TYPE::VERTEX].str(), ss[(int)SHADER_TYPE::FRAGMENT].str());
}