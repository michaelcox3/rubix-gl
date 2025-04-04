#pragma once

#include "Renderer.h"

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; // BPP = Bits per pixel

public:
	Texture(const std::string& path);
	~Texture();

	// optional slot param to specify which slot to bind the texture to
	// we can bind more than 1 texture at a time
	// windows supports 32 texture slots at one time
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};