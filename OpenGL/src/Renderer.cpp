#include "Renderer.h"
#include <iostream>

// forces a breakpoint, stop the debugger
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// TODO CONVERT ERROR DECIMAL TO HEXADECIMAL THAN TO THE GL ERROR ENUM TEXT
bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): "
			<< function << " "
			<< file << ":"
			<< line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const {
	shader.Bind();
	vao.Bind();
	ibo.Bind();

	// HAS TO BE UNSIGNED INT
	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); //draw for index buffers

}