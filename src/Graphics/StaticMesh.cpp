#include "StaticMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>


StaticMesh::StaticMesh(const std::vector<Vertex>& verts)
	: vertices(verts)
{
}

void StaticMesh::Create()
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangent)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Bitangent)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//vertices.clear();
}

void StaticMesh::Bind(Graphics& gfx)
{
	glBindVertexArray(vao);
}

void StaticMesh::Unbind(Graphics& gfx)
{
	glBindVertexArray(0);
}

StaticMesh::StaticMesh(const std::string& file_path) : path(file_path)
{
	std::vector<unsigned int> indices;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

	if (!warn.empty())
		std::cout << "MESH WARNING: " << path << " : " << warn << "\n";
	if (!err.empty())
		std::cout << "MESH ERROR: " << path << " : " << warn << "\n";
	if (!ret)
	{
		std::cout << "MESH ERROR: FAILED TO LOAD MESH " << path << ", CHECK RESOURCE FOLDER\n";
	}
	size = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (int v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

				Vertex vert;
				vert.Position = { vx,vy,vz };
				vert.Normal = { nx,ny,nz };
				vert.TexCoords = { tx,ty };

				vertices.push_back(vert);
				size++;
				//indices.push_back(idx.vertex_index);
			}
			auto iv1 = vertices.size() - 3;
			glm::vec3 v1 = vertices[iv1 + 0].Position;
			glm::vec3 v2 = vertices[iv1 + 1].Position;
			glm::vec3 v3 = vertices[iv1 + 2].Position;
			glm::vec2 uv1 = vertices[iv1 + 0].TexCoords;
			glm::vec2 uv2 = vertices[iv1 + 1].TexCoords;
			glm::vec2 uv3 = vertices[iv1 + 2].TexCoords;

			auto e1 = v2 - v1;
			auto e2 = v3 - v1;

			auto duv1 = uv2 - uv1;
			auto duv2 = uv3 - uv1;

			float det = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);

			glm::vec3 t, b;

			t.x = det * (duv2.y * e1.x - duv1.y * e2.x);
			t.y = det * (duv2.y * e1.y - duv1.y * e2.y);
			t.z = det * (duv2.y * e1.z - duv1.y * e2.z);
			t = glm::normalize(t);

			b.x = det * (-duv2.x * e1.x + duv1.x * e2.x);
			b.y = det * (-duv2.x * e1.y + duv1.x * e2.y);
			b.z = det * (-duv2.x * e1.z + duv1.x * e2.z);
			b = glm::normalize(b);

			vertices[iv1 + 0].Tangent = t;
			vertices[iv1 + 1].Tangent = t;
			vertices[iv1 + 2].Tangent = t;

			vertices[iv1 + 0].Bitangent = b;
			vertices[iv1 + 1].Bitangent = b;
			vertices[iv1 + 2].Bitangent = b;

			index_offset += fv;
		}
	}
	std::cout << "Loaded StaticMesh: " << path << "\n";
}

void StaticMesh::Draw(Graphics& gfx)
{
	Bind(gfx);
	glDrawArrays(GL_TRIANGLES, 0, size);
	Unbind(gfx);
}
