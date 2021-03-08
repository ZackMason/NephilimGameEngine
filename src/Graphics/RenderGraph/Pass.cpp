#include "Pass.h"

#include "Sink.h"
#include "Source.h"

namespace rg
{
	Source& Pass::GetSource(const std::string& source_name) const
	{
		for (auto& s : sources)
		{
			if (s->name == source_name)
				return *s;
		}
	}

	Sink& Pass::GetSink(const std::string& sink_name) const
	{
		for (auto& s : sinks)
		{
			if (s->name == sink_name)
				return *s;
		}
	}

	void Pass::RegisterSink(Unique<Sink> sink)
	{
		for (auto& s : sinks)
		{
			if (s->name == sink->name)
			{
				assert(0);
			}
		}
		sinks.push_back(std::move(sink));
	}

	void Pass::RegisterSource(Unique<Source> source)
	{
		for (auto& s : sources)
		{
			if (s->name == source->name)
			{
				assert(0);
			}
		}
		sources.push_back(std::move(source));
	}

	void Pass::LinkSink(const std::string& sink_name, const std::string& target)
	{
	}

	void Pass::Finalize()
	{
	}
}