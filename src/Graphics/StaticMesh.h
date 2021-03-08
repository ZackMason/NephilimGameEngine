#pragma once

#include "../Engine/Core.h"

#include "Bindable.h"
#include "Graphics.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct StaticMesh : Drawable
{
	StaticMesh(const std::string& file_path);
	StaticMesh(const std::vector<Vertex>& verts);

	void LoadVBO();
	void Create();
	void Draw(Graphics& gfx) override;
	void Bind(Graphics& gfx) override;
	void Unbind(Graphics& gfx) override;

	GLuint vbo;
	GLuint vao;
	std::size_t size;
	std::string path;

	std::vector<Vertex> vertices;
};