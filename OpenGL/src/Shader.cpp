#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath) {
	// relative to the project directory, defined in debugging
	std::string vertexShader = Shader::ParseShader("res/shaders/Vertex.shader");
	std::string fragShader = Shader::ParseShader("res/shaders/Frag.shader");
	std::cout << vertexShader << std::endl;
	std::cout << fragShader << std::endl;
	m_RendererID = Shader::CreateShaderProgram(vertexShader, fragShader);
	glUseProgram(m_RendererID);
}

Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3) {
	int location = Shader::GetUniformLocation(name);
	GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, const float value) {
	GLCall(glUniform1f(Shader::GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, const int value) {
	GLCall(glUniform1i(Shader::GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix) {
	// 1 means sending 1 matrix
	// GL_FALSE means no transpose (GLM already stores the data in column-major format which OpenGL expects)
	//			if our matrix data was stored in row-major format, then we would need to transpose the matrix
	// const GLfloat *value : OpenGL wants a pointer to the 0th 0th row column of the matrix
	// 
	GLCall(glUniformMatrix4fv(Shader::GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::GetUniformLocation(const std::string& name) {

	// First check uniform cache
	auto iter = m_UniformLocationCache.find(name);
	if (iter != m_UniformLocationCache.end()) {
		return iter->second; // second is the value, first is the key (string) 
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	//ASSERT(location != -1);

	// Add to the cache
	m_UniformLocationCache[name] = location;
	return location;
}

int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); // create shader program, instead of taking in the ref to the int like in genbuffer, it returns the id
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program); // code uwrite once and forget about it :D
	glValidateProgram(program); // performs validation on the program .. XD

	// now that the shaders had been linked into a program, delete the intermediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // or &source[0] same thing

	// if length = null, it means each string is assumed to be null terminated. (\0)
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

std::string Shader::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);

	bool isStartOfShaderCode = false;

	std::string line;
	std::stringstream ss;
	while (getline(stream, line)) {
		// npos means substring not found
		if (line.find("#shader") != std::string::npos) {
			isStartOfShaderCode = true;
		}
		else if (isStartOfShaderCode) {
			ss << line << '\n';
		}
	}

	return ss.str();
}