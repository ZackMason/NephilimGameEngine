#pragma once

#include "../../Engine/Core.h"
#include "../Graphics.h"

namespace rg
{
struct Sink;
struct Source;
	struct Pass
	{
		std::string name;
		std::vector<Unique<Sink>> sinks;
		std::vector<Unique<Source>> sources;

		Pass(std::string pass_name) : name(std::move(pass_name))
		{
		}
		virtual ~Pass() = default;

		virtual void Execute(Graphics& gfx) const = 0;
		virtual void Reset() const = 0;

		Source& GetSource(const std::string& source_name) const;
		Sink& GetSink(const std::string& sink_name) const;

		void RegisterSink(Unique<Sink> sink);
		void RegisterSource(Unique<Source> source);

		void LinkSink(const std::string& sink_name, const std::string& target);
		virtual void Finalize();
	};
}