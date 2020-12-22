#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "glm/glm.hpp"

#include "Renderer.h"

enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1,
	GEOMETRY = 2,
	NUM_TYPES
};

class Shader
{
private:
	unsigned int m_RendererId;
	std::string m_Filepath;
	std::map<ShaderType, GLenum> shaderTypeMap = {
		{ShaderType::VERTEX, GL_VERTEX_SHADER},
		{ShaderType::FRAGMENT, GL_FRAGMENT_SHADER},
		{ShaderType::GEOMETRY, GL_GEOMETRY_SHADER}
	};
	std::unordered_map<std::string, int> m_UniformLocCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	void SetUniform3f(const std::string& name, float f1, float f2, float f3);
	void SetUniform2f(const std::string& name, float f1, float f2);
	void SetUniform1f(const std::string& name, float f1);
	void SetUniform1i(const std::string& name, unsigned int val);
	void SetUniform1iv(const std::string& name, unsigned int size, int* vals);

private:
	const int GetUniformLocation(const std::string& name);
	std::tuple<std::string, std::string> ParseShader(const std::string& srcPath);
	std::map<ShaderType, std::string> ParseShaders(const std::string& srcPath);
	GLuint CompileShader(GLenum shaderType, const std::string& src);
	GLuint CreateShaderProgram(const std::string& vertSrc, const std::string& fragSrc);
	GLuint CreateShaderProgram(const std::string& filepath);
	GLuint CreateShaderProgram(const std::map<ShaderType, std::string> sourceMap);

};
