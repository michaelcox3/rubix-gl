#pragma once

#include <GL/glew.h>
#include "IndexBufferObject.h"
#include "VertexArrayObject.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer {
private:

public:
	void Clear() const;
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
};