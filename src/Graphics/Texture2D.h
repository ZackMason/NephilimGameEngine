#pragma once

#include "Texture.h"

struct Texture2D : Texture
{
	Texture2D() = default;
	Texture2D(int width, int height);
	Texture2D(const std::string&);
	virtual ~Texture2D();

	void SetData(void* data, uint32_t size);
	void Bind(Graphics& gfx) override;
	void Unbind(Graphics& gfx) override;

	int m_Width;
	int m_Height;
	int m_Channels;
	GLenum m_InternalFormat, m_DataFormat;
};

