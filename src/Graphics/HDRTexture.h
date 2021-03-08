#pragma once

#include "Texture.h"

struct HDRTexture : Texture
{
	HDRTexture(const std::string& file);
	virtual ~HDRTexture() = default;

	void Bind(Graphics& gfx) override;
	void Unbind(Graphics& gfx) override;

	unsigned int prefilterMap;
	unsigned int irradiance_map;
	unsigned int cube_map;
	unsigned int brdfLUTTexture;
};