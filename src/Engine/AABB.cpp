#include "AABB.h"

#include <numeric>
#include <limits>

#include "../Graphics/StaticMesh.h"

void AABB::MakeFromMesh(const std::vector<Vertex>& vertices)
{
	min = v3(std::numeric_limits<float>::max());
	max = v3(std::numeric_limits<float>::min());
	for (const auto& v : vertices)
	{
		min = glm::min(min, v.Position);
		max = glm::max(max, v.Position);
	}
}
