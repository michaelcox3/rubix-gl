#include "VertexArrayObject.h"
#include "Renderer.h"


VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	// glVertexAttribPointer: defines the structure of the data. tells opengl how to interpret the data
	// ex. the number of components per vertex, the data type, and stride 
	// * ITS THIS LINE THAT LINKS THE BUFFER ABOVE TO THE VAO
	// 
	// index = 0 refers to the first attribute list of vao is going to be bound to the currently bound VBO 
	// size = component count in the attribute (the count NOT THE SIZE IN BYTES..)
	// type = float
	// GL_FALSE because no already in floats (already normalized)
	// stride = # of bytes (2 floats = 8 bytes) (the byte offset between consecutive vertex attributes; here, the vertices are tightly packed with two floats each).
	// pointer = offset of the 1st component of the first generic vertex attribute (offset in buffer to where the data begins)
	unsigned int offset = 0;
	const auto& elements = layout.GetElements();
	for (int i = 0; i < elements.size(); i++) {
		const VertexBufferElement vbl = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, vbl.count, vbl.type, vbl.normalized, layout.GetStride(), (const void*) offset));
		offset += vbl.count * sizeof(vbl.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}