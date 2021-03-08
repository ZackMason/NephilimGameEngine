#pragma once 

#include "glad/glad.h"

#include "UniformBuffer.h"

#include "../Engine/Camera.h"

struct Graphics
{
	void ClearColor(f32 r, f32 g, f32 b, f32 a) {
		glClearColor(r, g, b, a);
	}
	void Clear(GLenum flag) {
		glClear(flag);
	}
	void CullMode(GLenum mode)
	{
		glCullFace(mode);
	}

	void Enable(GLenum flag)
	{
		glEnable(flag);
	}

	void SetCamera(uCamera& camera_data)
	{
		camera_ubo.SetBuffer(camera_data);
	}

	UniformBufferObject camera_ubo;
	Graphics() : camera_ubo("uCamera")
	{
		std::cout << "Initalizing Graphics..\n";

		camera_ubo.Init<uCamera>(1, GL_DYNAMIC_DRAW);
		camera_ubo.Bind(*this);
	}
};