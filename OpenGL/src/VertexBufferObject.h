#pragma once
// Abstraction for a Vertex Buffer Object (VBO)
class VertexBuffer
{
private:
	unsigned int m_RendererID; // id to the vertex buffer
public:
	// Constructor: uploads data to the GPU
	VertexBuffer(const void* data, unsigned int size); // data is a generic pointer, can be a pointer to any type of data
	// Destructor: cleans up GPU resources
	~VertexBuffer(); // called when 'delete myVBuffer'

	void Bind() const;
	void Unbind() const;
};

