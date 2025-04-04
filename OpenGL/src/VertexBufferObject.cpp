#include "VertexBufferObject.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	// Define a Vertex Buffer Object
	unsigned int buffer;
	// n: specifies number of buffer object names to be generated
	// buffers: an array in which the generated buffer object names are stored (if only generating 1, it can just be an unsigned int
	GLCall(glGenBuffers(1, &buffer)); // this creates the buffer, returns the id

	// Because OpenGL is a 'state machine' , we must call glBindBuffer to specificy which buffer we are refering to for subsequent operations
	// Selecting is known as binding in OpenGL (im about to work on this buffer)
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	// glBufferData: load the data into the buffer currently bound (from cpu into gpu)
	// target: the array buffer
	// size: the size of the data
	// data: the data
	// usage: GL_STATIC_DRAW is a usage param to define how this buffer will be used, specifically that is will be written to once, but used many times for drawing
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

	m_RendererID = buffer;
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
