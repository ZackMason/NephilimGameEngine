#include <iostream>

#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(const std::string& file)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	float aniso = 16.;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture

	const std::string path = "./res/TEXTURES/" + file;

	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
	if (data)
	{
		if (m_Channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (m_Channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "LOADED TEXTURE: " << file << "\n";
	}
	else
	{
		data = stbi_load("./res/TEXTURES/uvGrid.jpg", &m_Width, &m_Height, &m_Channels, 0);
		if (!data)
			goto j;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	j:
		std::cout << "Failed to load texture "<<  file <<"\n";
	}
	stbi_image_free(data);
}

Texture2D::Texture2D(const int width, const int height)
	: m_Width(width), m_Height(height)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, nullptr);
	//glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::SetData(void* data, uint32_t size)
{
	const uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	assert(size == m_Width * m_Height * bpp);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &texture);
}

void Texture2D::Bind(Graphics& gfx)
{
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture2D::Unbind(Graphics& gfx)
{
	assert(slot >= 0 && slot < 32);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
