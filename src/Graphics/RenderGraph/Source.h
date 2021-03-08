#pragma once

#include "../../Engine/Core.h"
#include "../Bindable.h"

namespace rg
{
	struct Source
	{
		std::string name;

		Source(std::string source_name, Ref<Bindable>& res) : name(std::move(source_name)), resource(res) {}

		virtual void ValidateLink() const = 0;
		virtual Ref<Bindable> GetResource() { return resource; }

		Ref<Bindable>& resource;
	};
}
