#include "Transform.h"

#include "glm/gtc/matrix_transform.hpp"

m4 Transform::ToMatrix() const
{
	m4 res = basis;
	res[3] = v4(origin,1.0);
	return res;
}

void Transform::Translate(const v3& delta)
{
	origin += delta;
}

void Transform::Scale(const v3& delta)
{
	basis = glm::scale(m4(basis), delta);
}

void Transform::Rotate(const v3& delta)
{
	m4 rot = m4(1.0f);
	rot = glm::rotate(rot, delta.z, { 0,0,1 });
	rot = glm::rotate(rot, delta.y, { 0,1,0 });
	rot = glm::rotate(rot, delta.x, { 1,0,0 });
	basis = m4(basis) * rot;
}


void Transform::Rotate(const q4& delta)
{

}

v3 Transform::Origin() const
{
	return origin;
}

Transform Transform::Inverse() const
{
	Transform transform;
	transform.basis = glm::transpose(basis);
	transform.origin = basis * -origin;
	return transform;
}

void Transform::SetFOV(f32 fov, f32 width, f32 height, f32 near, f32 far)
{
	auto mat = glm::perspectiveFov(fov, width, height, near, far);
	basis = mat;
	origin = mat[3];
}

void Transform::LookAt(const v3& target, const v3& up)
{
	auto mat = glm::lookAt(origin, target, up);
	basis = mat;
	origin = mat[3];
}

void Transform::SetPosition(const v3& position)
{
	origin = position;
}

void Transform::SetScale(const v3& scale)
{
	for (int i = 0; i < 3; i++)
		basis[i] = glm::normalize(basis[i]) * scale[i];
}

void Transform::SetRotation(const v3& rotation)
{
}

void Transform::SetRotation(const q4& quat)
{
}
