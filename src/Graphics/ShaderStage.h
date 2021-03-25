#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <functional>


class ShaderStage
{
public:
	ShaderStage() = delete;
	virtual ~ShaderStage();
	ShaderStage(const std::string& file);

	void LoadShader(const std::string& fileName);

	int m_type;
	int m_stage;
};

