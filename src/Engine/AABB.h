#pragma once
#include "Core.h"
#include "IntersectData.h"
#include "Ray.h"

struct Vertex;

struct AABB
{
	v3 min;
	v3 max;

	void MakeFromMesh(const std::vector<Vertex>& vertices);

	bool Contains(const v3& p)
	{
		return (p.x < max.x && p.x > min.x) && 
			(p.y < max.y && p.y > min.y) &&
			(p.z < max.z && p.z > min.z);
	}

	// todo add time?
	IntersectData IntersectRay(const Ray& ray, Node* node = nullptr)
	{
		if (Contains(ray.origin)) return { true , 0.0, node, 0.0};

		v3 inv_dir = 1.0f / ray.direction;

		v3 min_t = (min - ray.origin) * inv_dir;
		v3 max_t = (max - ray.origin) * inv_dir;

		f32 tmin = std::max(std::max(std::min(min_t.x, max_t.x), std::min(min_t.y, max_t.y)), std::min(min_t.z, max_t.z));
		f32 tmax = std::min(std::min(std::max(min_t.x, max_t.x), std::max(min_t.y, max_t.y)), std::max(min_t.z, max_t.z));

		if (tmax < 0.0f)
		{
			return { false, 0, nullptr, 0.0 };
		}
		if (tmin > tmax)
		{
			return { false, 0, nullptr, 0.0 };
		}
		return { true, tmin, node, 0.0, ray.origin + ray.direction*tmin };
	}
};
