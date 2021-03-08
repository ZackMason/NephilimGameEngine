#pragma once

#include "Pass.h"


namespace rg
{
	struct RenderGraph
	{
		void Execute(Graphics& gfx);
		void Reset();
		
		void SetSinkTarget(const std::string sink_name, const std::string& target);
		void AddGlobalSource(Unique<Source> source);
		void AddGlobalSink(Unique<Sink> source);
		void Finalize();
		void AppedPass(Unique<Pass> pass);
		void LinkSinks(Pass& pass);
		void LinkGlobalSinks();

		std::vector<Unique<Pass>> passes;
		std::vector<Unique<Source>> global_sources;
		std::vector<Unique<Sink>> global_sinks;
		
		// output fbo
	};
}