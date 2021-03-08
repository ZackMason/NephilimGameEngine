#pragma once

#include "../../Engine/Core.h"

namespace rg
{
	struct Source;

	struct Sink
	{
		std::string name;
		std::string pass_name;
		std::string source_name;

		virtual void Bind(Source& source) = 0;
		virtual void ValidateLink() const = 0;

		Sink(std::string sink_name) : name(std::move(sink_name)) {}
		virtual ~Sink() = default;
	};

	template <typename T>
	struct BindableSink : Sink
	{
		Ref<T>& bindable;
		bool linked = false;

		BindableSink(std::string sink_name, Ref<T>& bind)
			: Sink(sink_name), bindable(bind)
		{
		}

		void ValidateLink()
		{
			assert(linked);
		}

		void Bind(Source& source) override
		{

			linked = true;
		}
	};
}