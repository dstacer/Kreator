#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <map>

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "Shader.h"


Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath)
{
    m_RendererId = CreateShaderProgram(ParseShaders(filepath));
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
    GlApiCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GlApiCall(glUseProgram(0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4)
{
    GLint loc = GetUniformLocation(name); 
    GlApiCall(glUniform4f(loc, f1, f2, f3, f4));
}

void Shader::SetUniform3f(const std::string& name, float f1, float f2, float f3)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniform3f(loc, f1, f2, f3));
}

void Shader::SetUniform2f(const std::string& name, float f1, float f2)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniform2f(loc, f1, f2));
}

void Shader::SetUniform1f(const std::string& name, float f1)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniform1f(loc, f1));
}

void Shader::SetUniform1i(const std::string& name, unsigned int val)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniform1i(loc, val));
}

void Shader::SetUniform1iv(const std::string& name, unsigned int size, int* vals)
{
    GLint loc = GetUniformLocation(name);
    GlApiCall(glUniform1iv(loc, size, vals));
}

const int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocCache.find(name) != m_UniformLocCache.end())
        return m_UniformLocCache[name];

    GlApiCall(GLint loc = glGetUniformLocation(m_RendererId, name.c_str()));
    // Uniform may exist in shader, but not have a location due to being unused
    if (loc == -1)
        std::cout << "Warning: Uniform '" << name << "' has no location." << std::endl;

    m_UniformLocCache[name] = loc;
    return loc;
}

// This function expects a file with exactly 2 shaders: Vertex and Fragment
std::tuple<std::string, std::string> Shader::ParseShader(const std::string& srcPath)
{
    std::ifstream instream(srcPath);
    std::stringstream sources[(int)ShaderType::NUM_TYPES];
    ShaderType type(ShaderType::NONE);

    int numShaders = 0;

    std::string line;
    while (getline(instream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;

        }
        else
        {
            sources[(int)type] << line << std::endl;
        }
    }

    return std::make_tuple(sources[0].str(), sources[1].str());
}

// This function handles an arbitray number of shaders in a single file
std::map<ShaderType, std::string> Shader::ParseShaders(const std::string& srcPath)
{
    std::ifstream instream(srcPath);
    std::stringstream sources[(int)ShaderType::NUM_TYPES];
    std::map<ShaderType, std::string> retMap;
    ShaderType type(ShaderType::NONE);

    std::string line;
    while (getline(instream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::GEOMETRY;

        }
        else
        {
            sources[(int)type] << line << std::endl;
        }
    }

    for (int i = 0; i < (int)ShaderType::NUM_TYPES; i++)
        if (!sources[i].str().empty())
            retMap[(ShaderType)i] = sources[i].str();

    return retMap;
}

GLuint Shader::CompileShader(GLenum shaderType, const std::string& src)
{
    GlApiCall(GLuint shaderId = glCreateShader(shaderType));
    const char* source = src.c_str();
    GlApiCall(glShaderSource(shaderId, 1, &source, nullptr));
    GlApiCall(glCompileShader(shaderId));

    GLint status;
    GlApiCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint msgLen;
        GlApiCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLen));
        char* msg = (char*)_malloca(msgLen * sizeof(char));
        GlApiCall(glGetShaderInfoLog(shaderId, msgLen, &msgLen, msg));
        std::string typeStr = (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
        std::cout << typeStr << " Shader compilation failed: " << msg << std::endl;
        GlApiCall(glDeleteShader(shaderId));

        return 0;
    }

    return shaderId;
}

GLuint Shader::CreateShaderProgram(const std::string& vertSrc, const std::string& fragSrc)
{
    GlApiCall(GLuint program = glCreateProgram());
    GLuint vertId = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragId = CompileShader(GL_FRAGMENT_SHADER, fragSrc);
    GlApiCall(glAttachShader(program, vertId));
    GlApiCall(glAttachShader(program, fragId));
    GlApiCall(glLinkProgram(program));
    GlApiCall(glValidateProgram(program));

    GlApiCall(glDeleteShader(vertId));
    GlApiCall(glDeleteShader(fragId));

    GlApiCall(glDetachShader(program, vertId));
    GlApiCall(glDetachShader(program, fragId));

    return program;
}

GLuint Shader::CreateShaderProgram(const std::string& filepath)
{
    GlApiCall(GLuint program = glCreateProgram());
    auto [vertSrc, fragSrc] = ParseShader(filepath);
    GLuint vertId = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragId = CompileShader(GL_FRAGMENT_SHADER, fragSrc);
    GlApiCall(glAttachShader(program, vertId));
    GlApiCall(glAttachShader(program, fragId));
    GlApiCall(glLinkProgram(program));
    GlApiCall(glValidateProgram(program));

    GlApiCall(glDeleteShader(vertId));
    GlApiCall(glDeleteShader(fragId));

    GlApiCall(glDetachShader(program, vertId));
    GlApiCall(glDetachShader(program, fragId));

    return program;
}

GLuint Shader::CreateShaderProgram(const std::map<ShaderType, std::string> sourceMap)
{
    GlApiCall(GLuint program = glCreateProgram());
    std::vector<GLuint> shaderHandles;
    for (const auto& mapEntry : sourceMap)
    {
        shaderHandles.push_back(
            CompileShader(shaderTypeMap[mapEntry.first], mapEntry.second)
        );
        GlApiCall(glAttachShader(program, shaderHandles.back()));
    }

    GlApiCall(glLinkProgram(program));
    GlApiCall(glValidateProgram(program));

    for (GLuint handle : shaderHandles)
    {
        GlApiCall(glDeleteShader(handle));
        GlApiCall(glDetachShader(program, handle));
    }

    return program;
}
