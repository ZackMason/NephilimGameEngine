#pragma once

#include "Transform.h"
#include "Ray.h"

struct uCamera
{
	glm::mat4 v;
	glm::mat4 p;
	glm::vec4 camPos;
};

struct Camera
{
	Camera(f32 f, f32 w, f32 h, f32 n, f32 fa);

	void LookAt(const v3& pos, const v3& up = { 0,1,0 });

	m4 ViewProjection() const
	{
		return projection * view;// .ToMatrix();
	}

	void MakeProjection();

	auto GetForward() const {
		return -glm::normalize(v3(glm::transpose(view)[2]));  //.basis[2]));
	}

	auto GetRight() const {
		return -glm::normalize(v3(glm::transpose(view)[0]));  //.basis[2]));
	}

	auto GetPosition()const {
		return position;// view.origin;
	}

	Ray GetScreenRay(u32 x, u32 y)
	{
		v4 ndc = {
			(2.0f * x) / width - 1.0f,
			1.0f - (2.0f * y) / height,
			-1.0f, 1.0f,
		};

		v4 ray_eye = glm::inverse(projection) * ndc;
		ray_eye.z = -1.0f;
		ray_eye.w = 0.0f;
		return { position, glm::normalize(v3(glm::inverse(view) * ray_eye)) };
	}

	f32 fov;
	f32 width;
	f32 height; 
	f32 near = 0.01f;
	f32 far = 1000.f;

	v3 position;
	m4 view;
	//Transform view;
	m4 projection;
};