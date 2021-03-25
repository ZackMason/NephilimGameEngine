#pragma once

#include "Shader.h"
#include "Bindable.h"

#include "../Engine/Core.h"
#include "../Engine/AssetDB.h"

struct Material : Bindable
{
	enum Flags
	{
		COMBINED_METALLIC = 1 << 0,
		TRIPLANAR = 1 << 1,
		NO_METALLIC_ROUGHNESS = 1 << 2,
	};
	u32 flags = NO_METALLIC_ROUGHNESS;

	std::string shader;
	std::string vertex;
	std::string fragment;


	void SetTransform(const m4& transform)
	{
		auto s = AssetDB::ResolveShader(shader);
		s->Bind();
		s->setMat4("uM", transform);
	}

	Material()
	{
	}
	virtual ~Material() = default;
};

struct PBRMaterial : Material
{
	f32 roughness = 0.5f;
	f32 metallic = 0.0f;

	v3 color = v3(1.0, 1.0, 1.0);

	std::string diffuse_texture = "white";
	std::string normal_texture = "black";
	std::string metallic_roughness_texture = "";
	std::string metallic_texture = "";
	std::string roughness_texture = "";

	PBRMaterial()
	{
		shader = "pbr";
		vertex = "pbr.vs";
		fragment = "pbr.fs";

		AssetDB::BuildShader(shader, { vertex, fragment });
	}

	void Bind(Graphics& gfx) override
	{
		auto s = AssetDB::ResolveShader(shader);
		s->Bind();
		s->setFloat("uRoughness", roughness);
		s->setFloat("uMetallic", metallic);
		s->setVec3("uColor", color);
		s->setInt("uFlags", flags);

		auto dt = AssetDB::ResolveTexture2D(diffuse_texture);
		auto nt = AssetDB::ResolveTexture2D(normal_texture);
		auto mrt = AssetDB::ResolveTexture2D(metallic_roughness_texture);
		auto mt = AssetDB::ResolveTexture2D(metallic_texture);
		auto rt = AssetDB::ResolveTexture2D(roughness_texture);
		s->setInt("uIrradianceMap", 11);

		auto enviro = AssetDB::ResolveHDRTexture("Etnies_Park_Center_3k.hdr");
		glActiveTexture(GL_TEXTURE0 + 11);
		glBindTexture(GL_TEXTURE_CUBE_MAP, enviro->irradiance_map);

		if (dt)
		{
			dt->SetTextureSlot(1);
			dt->Bind(gfx);
		}
		if (nt)
		{
			nt->SetTextureSlot(2);
			nt->Bind(gfx);
		}
		if (mrt)
		{
			mrt->SetTextureSlot(3);
			mrt->Bind(gfx);
		}
		else {
			if (roughness_texture != "") {
				rt->SetTextureSlot(4);
				rt->Bind(gfx);
				s->setInt("uRoughBound", true);
			}
			else
				s->setInt("uRoughBound", false);
			if (metallic_texture != "") {
				mt->SetTextureSlot(3);
				mt->Bind(gfx);
				s->setInt("uMetalBound", true);
			}
			else
				s->setInt("uMetalBound", false);
		}
		s->setInt("uTexture1", 1);
		s->setInt("uTexture2", 2);
		s->setInt("uTexture3", 3);
		s->setInt("uTexture3", 4);
	}
};


struct WidgetMaterial : Material
{

	std::string diffuse_texture = "3DWidget_Pallet.png";

	WidgetMaterial()
	{
		shader = "widget";
	}

	void Bind(Graphics& gfx) override
	{
		auto s = AssetDB::ResolveShader(shader);
		s->Bind();

		auto dt = AssetDB::ResolveTexture2D(diffuse_texture);

		if (dt)
		{
			dt->SetTextureSlot(1);
			dt->Bind(gfx);
		}
		
		s->setInt("uTexture1", 1);
	}
};
