#pragma once

#include "Core.h"
#include "AABB.h"

struct Transform
{
	Transform() = default;
	Transform(const m4& mat) : basis(mat), origin(mat[3]) {};
	Transform(const v3& position, const v3& scale, const v3& rotation)
	{
		SetPosition(position);
		SetScale(scale);
		SetRotation(position);
	}

	m4 ToMatrix() const;

	operator m4() const {
		return ToMatrix();
	}

	void Translate(const v3& delta);
	void Scale(const v3& delta);
	void Rotate(const v3& delta);
	void Rotate(const q4& delta);

	v3 Origin() const;

	Transform Inverse() const;

	void SetFOV(f32 fov, f32 width, f32 height, f32 near, f32 far);
	void LookAt(const v3& target, const v3& up = { 0,1,0 });

	void SetPosition(const v3& position);
	void SetScale(const v3& scale);
	void SetRotation(const v3& rotation);
	void SetRotation(const q4& quat);

	v3 xform(const v3& vector)const
	{
		return v3(basis * vector) + origin;
	}

	AABB xform(const AABB& box)const
	{
		AABB t_box;
		t_box.min = t_box.max = origin;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				auto a = basis[j][i] * box.min[j];
				auto b = basis[j][i] * box.max[j];
				t_box.min[i] += a < b ? a : b;
				t_box.max[i] += a < b ? b : a;
			}

		//t_box.min = xform(box.min);
		//t_box.max = xform(box.max);
		return t_box;
	}

	m3 basis = m3(1.0f);
	v3 origin = v3(0, 0, 0);
};
