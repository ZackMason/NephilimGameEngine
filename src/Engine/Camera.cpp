#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(f32 f, f32 w, f32 h, f32 n, f32 fa)
	:
	fov(f), width(w), height(h), near(n), far(fa)
{
	projection = glm::perspectiveFov(fov, width, height, near, far);
}

void Camera::LookAt(const v3& pos, const v3& up)
{
	//view.LookAt(pos, up);
	view = glm::lookAt(position, pos, up);
}

void Camera::MakeProjection()
{
	projection = glm::perspectiveFov(fov, width, height, near, far);
}
