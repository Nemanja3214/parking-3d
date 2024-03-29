#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include <GL/glew.h>   

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_vertexShaderFilePath;
    std::string m_fragmentShaderFilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    unsigned int getId();

    // Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform3f(const std::string& name, const glm::vec3& vec);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUniform4fv(const std::string& name, const glm::vec4& vec);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    int GetUniformLocation(const std::string& name);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(GLenum type, const char* source);
    unsigned int createShader(const char* vsSource, const char* fsSource);

};