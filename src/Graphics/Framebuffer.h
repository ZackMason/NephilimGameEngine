#pragma once
#include "glad/glad.h"

#include "Bindable.h"
#include "../Engine/Core.h"


enum class eFramebufferMode
{
	RENDER_TARGET, TEXTURE, SIZE
};

struct Framebuffer : Bindable
{
	Framebuffer(int width, int height, bool _msaa = false);
	~Framebuffer();

	void Bind(Graphics& gfx) override;
	void Unbind(Graphics& gfx) override;
	void Clear(Graphics& gfx, const v4& color = { 0.0, 0.0, 0.0, 0.0 }, GLenum flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	void Resize(u32 w, u32 h);
	void Blit(Framebuffer& msaa_fbo);

	eFramebufferMode mode = eFramebufferMode::RENDER_TARGET;

	u32 width, height;

	bool msaa;

	GLuint m_tbo;
	GLuint m_rbo;
	GLuint m_fbo;
	GLuint slot;
	bool resize = true;
};

