#pragma once

#include "Node3D.h"
#include "../../Graphics/StaticMesh.h"
#include "../../Graphics/Material.h"
#include "../AABB.h"

#include "ImGUI/imgui_stdlib.h"

struct StaticMeshNode : Node3D
{
	std::string mesh;
	Ref<Material> material = CreateRef<PBRMaterial>();

	AABB bounding_box;

	std::string file_path;

	void Edit(Editor& editor) override
	{
		Node3D::Edit(editor);

		ImGui::Separator();

		ImGui::InputText("mesh_path", &mesh);
		auto mat = std::dynamic_pointer_cast<PBRMaterial>(material);
		ImGui::ColorPicker3("Albedo", glm::value_ptr(mat->color), ImGuiColorEditFlags_PickerHueWheel);
		ImGui::DragFloat("roughness", &mat->roughness, 0.1, 0.0, 1.0);
		ImGui::DragFloat("metallic", &mat->metallic, 0.1, 0.0, 1.0);

		ImGui::Separator();
		ImGui::Text("Texture Properties");
		ImGui::InputText("albedo", &mat->diffuse_texture);
		ImGui::InputText("normal", &mat->normal_texture);
		ImGui::InputText("metalic/roughness", &mat->metallic_roughness_texture);

	}

	void OnDraw(Graphics& gfx) override
	{
		if (mesh != "")
		{
			auto m = AssetDB::ResolveStaticMesh(mesh);
			material->SetTransform(local_transform.ToMatrix());
			material->Bind(gfx);
			
			m->Draw(gfx);
		}

		if (0)
		{
			auto box = local_transform.xform(bounding_box);
			auto box_mesh = AssetDB::ResolveStaticMesh("res/MODELS/cube.obj");
			Transform box_t;
			box_t.Scale((box.max - box.min)/2.0f);
			box_t.origin = (box.max + box.min) / 2.0f;
			material->SetTransform(box_t.ToMatrix());
			material->Bind(gfx);
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			box_mesh->Draw(gfx);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	IntersectData IntersectRay(const Ray& ray) override
	{
		if (auto hit_data = local_transform.xform(bounding_box).IntersectRay(ray, this); hit_data.intersect)
		{
			return hit_data;
		}
		else
		{
			return Node::IntersectRay(ray);
		}
	}

	void SetMesh(const std::string& m)
	{
		mesh = m;
		bounding_box.MakeFromMesh(AssetDB::ResolveStaticMesh(mesh)->vertices);
		
	}

	StaticMeshNode(const std::string& node_name, const std::string& mesh_name)
	{
		name = node_name;
		SetMesh(mesh_name);
	}


	StaticMeshNode() 
	{
		name = "StaticMesh";
	}

	NODEDESC(StaticMeshNode, Node3D);

};