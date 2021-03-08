#pragma once
#include <glad/glad.h>

#include <string>

#include "Bindable.h"

enum class eTextureType
{
	TEX2D,
	TEX3D,
	HDR,
	SIZE
};

struct Texture : Bindable
{
	Texture() = default;
	~Texture() = default;
	GLuint texture;
	GLuint slot;

	void SetTextureSlot(GLuint unit)
	{
		slot = unit;
	}

	eTextureType type = eTextureType::SIZE;
	virtual void Bind(Graphics& gfx) = 0;
	virtual void Unbind(Graphics& gfx) = 0;
};


