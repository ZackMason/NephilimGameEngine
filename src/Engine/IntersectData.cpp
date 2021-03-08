#include "IntersectData.h"

#include "Node/Node.h"

std::ostream& operator<<(std::ostream& os, const IntersectData& data)
{
	if (data.intersect)
	{
		os << "RAYCAST::Node: " << (data.node ? data.node->name : "Null") << " (" << data.node << "), distance: " << 
			data.distance << ", position: " << data.position;
	}
	else
	{
		os << "RAYCAST::No Intersection";
	}
	return os;
}
