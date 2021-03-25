#pragma once

#include "ShaderStage.h"

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>	

const std::regex r("(#include <)([a-zA-Z]+)(\.slib>)");

void IncludePreprocess(std::string& code)
{
	std::smatch m;
	while (std::regex_search(code, m, r))
	{
		std::ifstream file;
		std::stringstream code_stream;
		file.open("./res/SHADERS/" + (std::string)m[2] + ".slib");
		if (file.is_open())
		{
			code_stream << file.rdbuf();
			file.close();
			std::regex file_reg("(#include <)" + (std::string)m[2] + "(\.slib>)");
			code = std::regex_replace(code, file_reg, code_stream.str());
		}
		else
			std::cout << "SHADER INCLUDE ERROR: file " << (std::string)m[2] << " not found\n";
	}
}

ShaderStage::ShaderStage(const std::string& file)
{
	LoadShader(file);
}

void ShaderStage::LoadShader(const std::string& file_name)
{
	if (file_name.substr(file_name.find_last_of(".") + 1) == "vs")
	{
		m_type = GL_VERTEX_SHADER;
	}
	else if (file_name.substr(file_name.find_last_of(".") + 1) == "fs")
	{
		m_type = GL_FRAGMENT_SHADER;
	}
	else if (file_name.substr(file_name.find_last_of(".") + 1) == "gs")
	{
		m_type = GL_GEOMETRY_SHADER;
	}
	else
	{
		assert(0); // shader type not supported
	}
	
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::ifstream vShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string vertexPath = "./res/SHADERS/" + file_name;
	try
	{
		// open 
		vShaderFile.open(vertexPath);
		std::stringstream vShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();

		IncludePreprocess(vertexCode);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << file_name << "\n";
	}

	const char* vShaderCode = vertexCode.c_str();

	int vertexShader = glCreateShader(m_type);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << file_name << "::VERTEX::COMPILATION_FAILED: " << infoLog << "\n";
		//TRIP_LOG_ERROR("ERROR::SHADER::{}::VERTEX::COMPILATION_FAILED: {}",file, infoLog);
	}
	else
	{
		m_stage = vertexShader;
	}
}

ShaderStage::~ShaderStage()
{
	glDeleteShader(m_stage);
}
