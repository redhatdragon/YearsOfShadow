#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <fstream>

#include <glm/glm.hpp>

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
    {
        HAL_LOG("Error: shader not loaded from path '{}'\n", fileName);
    }
}
void Shader::destruct() {
    GL_CALL(glDeleteProgram(id));
}
inline void Shader::bind() {
    GL_CALL(glUseProgram(id));
}
inline void Shader::unbind() {
    GL_CALL(glUseProgram(0));
}
inline void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}
inline void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
    GL_CALL(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
inline void Shader::setUniform1i(const std::string& name, int32_t v) {
    GL_CALL(glUniform1i(getUniformLocation(name), v));
}
inline void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}



//Private
inline int32_t Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];
    GL_CALL(auto retValue = glGetUniformLocation(id, name.c_str()));
    if (retValue == -1)
    {
        HAL_WARN("Warning: uniform '{}' doesn't exist.\n", name);
    }
    uniformLocationCache[name] = retValue;
    return retValue;
}
int Shader::compileShader(uint32_t type, const std::string& source) {
    GL_CALL(uint32_t id = glCreateShader(type));
    const char* src = source.c_str();
    const int srcLen = source.size();
    GL_CALL(glShaderSource(id, 1, &src, &srcLen));
    GL_CALL(glCompileShader(id));
    //TODO: error handling
    int result = GL_FALSE;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    int length;
    GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    if (length > 0) {
        char* message = (char*)alloca(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(id, length, &length, message));
        std::string shaderTypeString;
        switch (type) {
        case GL_VERTEX_SHADER:
            shaderTypeString = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderTypeString = "fragment";
            break;
        }
        HAL_ERROR("Shader of type '{}' has failed to compile\n'{}'\nSource: '{}'\n", shaderTypeString, message, source);
        GL_CALL(glDeleteShader(id));
        return 0;
    }
    return id;
}
int Shader::createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    GL_CALL(uint32_t program = glCreateProgram());
    uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));
    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));
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
                HAL_ERROR("Error: unkown shader type!\n");
                return -1;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return Shader::createShaderProgram(ss[(int)SHADER_TYPE::VERTEX].str(), ss[(int)SHADER_TYPE::FRAGMENT].str());
}