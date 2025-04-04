#pragma once

#include <vector>
#include <GL/glew.h>
#include "GLType.h"

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	// the count refers to the 
	template<typename T>
	void Push(unsigned int count) 
	{
		m_Elements.push_back({ GLType<T>::Get(), count, GL_FALSE});
		// possible source of bug because we are assuming the c++ type matches with the OpenGL type
		// ex. Will float == GL_FLOAT  always hold true on all systems?
		m_Stride += count * sizeof(T); 
	}

	// const std::vector<VertexBufferElement>& means the function returns a reference to a vector of VertexBufferElement objects
	// the const after the parameters specifies that this member function will not modify any member variables
	// The trailing & is a reference qualifier.
	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};

