#pragma once 

#include "Core.h"

#include <random>

struct Random
{
	Random() : re(std::random_device()())
	{
	}

	v3 rand_box(float radius)
	{
		return v3(
			randf(radius * 2.0f) - radius,
			randf(radius * 2.0f) - radius,
			randf(radius * 2.0f) - radius);
	}

	v3 rand_color()
	{
		return v3(
			randf(),
			randf(),
			randf()
		);
	}

	v3 rand_circle(float radius, bool fill = false)
	{
		v3 dir(
			randf(radius * 2.0f) - radius,
			randf(radius * 2.0f) - radius,
			randf(radius * 2.0f) - radius);
		const float dist = std::sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z* dir.z);
		return dir / dist * (fill ? randf(radius) : radius);
	}


	void seed(uint32_t s)
	{
		re.seed(s);
	}

	operator float()
	{
		return randf();
	}

	size_t rand_range(size_t start, size_t end)
	{
		return (randi() % (end - start)) + start;
	}

	uint32_t randi(uint32_t max = std::numeric_limits<uint32_t>::max())
	{
		return dist(re) % max;
	}

	float randf(float scale = 1.0f)
	{
		return (static_cast<float>(dist(re)) /
			static_cast<float>(std::numeric_limits<uint32_t>::max())) * scale;
	}

	std::mt19937 re;
	std::uniform_int_distribution<std::mt19937::result_type> dist;
};

