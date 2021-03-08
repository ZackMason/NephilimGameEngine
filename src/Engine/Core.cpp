#pragma once 

#include "Core.h"

std::ostream& operator << (std::ostream& os, const v3& v)
{
	os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return os;
}