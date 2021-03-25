#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>	

#include "Shader.h"
#include "Engine/AssetDB.h"

Shader::Shader(const std::string& name, const std::vector<std::string>& files)
	:
	m_Name(name)
{
	m_Program = glCreateProgram();
	
	for (const auto& file : files)
	{
		auto stage = AssetDB::ResolveShaderStage(file);
		glAttachShader(m_Program, stage->m_stage);
	}
	int success;
	char infoLog[512];

	glLinkProgram(m_Program);
	// check for linking errors
	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		//TRIP_LOG_ERROR("ERROR::SHADER::{}::PROGRAM::LINKING_FAILED: {}", file, infoLog);
		std::cout << "ERROR::SHADER::" << name << "::PROGRAM::LINKING_FAILED: " << infoLog << "\n";
	}
	else
	{
		std::cout << "LOADED SHADER : " << name << "\n";
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_Program);
}

void Shader::Bind()
{
	glUseProgram(m_Program);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	auto itr = m_uniform_location_cache.find(name);
	if ( itr != m_uniform_location_cache.end())
		return itr->second;
	GLint location = glGetUniformLocation(m_Program, name.c_str());
	m_uniform_location_cache[name] = location;
	return location;
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setUniformBlock(const std::string& name, int loc) const
{
  auto index = glGetUniformBlockIndex(m_Program, name.c_str());
  if (index != GL_INVALID_INDEX)
  glUniformBlockBinding(m_Program, index, loc);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
void Shader::setUint(const std::string& name, unsigned int value) const
{
	glUniform1ui(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}
#if 1
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(GetUniformLocation(name), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(GetUniformLocation(name), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
#endif
