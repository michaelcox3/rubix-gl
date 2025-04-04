#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, const int value);
	void SetUniform1f(const std::string& name, const float value);
	void SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

private:
	int GetUniformLocation(const std::string& name);

	std::string ParseShader(const std::string& filePath);
	int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
};


