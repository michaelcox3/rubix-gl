#pragma once
// Abstraction for a Index Buffer Object (VBO)
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count; // how many indices
public:
	IndexBuffer(const unsigned int* data, unsigned int count); // element count
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

