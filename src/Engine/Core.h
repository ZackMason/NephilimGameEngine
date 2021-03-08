#pragma once 

#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <thread>
#include <ctype.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8  = int8_t;

using f32 = float;
using f64 = double;

using v2 = glm::vec2;
using v3 = glm::vec3;
using v4 = glm::vec4;

using m2 = glm::mat2;
using m3 = glm::mat3;
using m4 = glm::mat4;

using q4 = glm::quat;


std::ostream& operator << (std::ostream& os, const v3& v);