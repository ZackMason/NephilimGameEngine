#pragma once
#include "Core.h"

#include "Graphics/Shader.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Texture2D.h"
#include "Graphics/HDRTexture.h"
#include "Graphics/StaticMesh.h"

struct AssetDB
{
	using ShaderCache = std::unordered_map<std::string, Ref<Shader>>;
	using FramebufferCache = std::unordered_map<std::string, Ref<Framebuffer>>;
	using Tex2DCache = std::unordered_map<std::string, Ref<Texture2D>>;
	using HDRCache = std::unordered_map<std::string, Ref<HDRTexture>>;
	using StaticMeshCache = std::unordered_map<std::string, Ref<StaticMesh>>;

	inline static ShaderCache shader_cache;
	inline static FramebufferCache frame_cache;
	inline static Tex2DCache tex2D_cache;
	inline static HDRCache hdr_cache;
	inline static StaticMeshCache static_mesh_cache;

	static Ref<HDRTexture> ResolveHDRTexture(const std::string& tex)
	{
		if (tex.empty()) return nullptr;
		Ref<HDRTexture> result;
		if (auto itr = hdr_cache.find(tex); itr == hdr_cache.end())
		{
			result = CreateRef<HDRTexture>(tex);
			hdr_cache[tex] = result;
		}
		else
		{
			result = itr->second;
		}

		return result;
	}


	static Ref<StaticMesh> ResolveStaticMesh(const std::string& mesh)
	{
		if (mesh.empty()) return nullptr;
		Ref<StaticMesh> result;
		auto itr = static_mesh_cache.find(mesh);
		if (itr == static_mesh_cache.end())
		{
			result = CreateRef<StaticMesh>(mesh);
			result->Create();
			static_mesh_cache[mesh] = result;
		}
		else
		{
			result = itr->second;
		}

		return result;
	}

	static Ref<Texture2D> ResolveTexture2D(const std::string& texture)
	{
		if (texture.empty()) return nullptr;
		Ref<Texture2D> result;
		auto itr = tex2D_cache.find(texture);
		if (itr == tex2D_cache.end())
		{
			result = CreateRef<Texture2D>(texture);
			tex2D_cache[texture] = result;
		}
		else
		{
			result = itr->second;
		}

		return result;
	}

	static void ResizeFramebuffers(u32 width, u32 height)
	{
		for (auto& [k, v] : frame_cache)
		{
			if(v->resize)
				v = CreateRef<Framebuffer>(width, height);
		}
	}

	static Ref<Framebuffer> CreateFramebuffer(const std::string& framebuffer, u32 width, u32 height, bool msaa = false)
	{
		auto itr = frame_cache.find(framebuffer);
		if (itr != frame_cache.end()) assert(0);

		auto fb = CreateRef<Framebuffer>(width, height, msaa);
		frame_cache[framebuffer] = fb;

		return fb;
	}

	static Ref<Framebuffer> ResolveFramebuffer(const std::string& framebuffer)
	{
		Ref<Framebuffer> result;
		auto itr = frame_cache.find(framebuffer);
		if (itr == frame_cache.end())
		{
			assert(0);
			// need to make a default framebuffer
		}
		else
		{
			result = itr->second;
		}

		return result;
	}

	static Ref<Shader> ResolveShader(const std::string& shader)
	{
		Ref<Shader> result;
		auto itr = shader_cache.find(shader);
		if (itr == shader_cache.end())
		{
			result = CreateRef<Shader>(shader);
			shader_cache[shader] = result;
		}
		else
		{
			result = itr->second;
		}

		return result;
	}

	static void ReloadShaderCache()
	{
		for (auto& [k, v] : shader_cache)
		{
			v = CreateRef<Shader>(k);
		}
	}

	static void SetShadersUniformBlock(const std::string& unifrom, int block)
	{
		for (auto& [k, v] : shader_cache)
		{
			v->setUniformBlock(unifrom, block);
		}
	}

};