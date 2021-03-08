#pragma once

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct Config
{
	json data;

	Config()
	{
		data["name"] = "Nephilim";
		data["version"] = "0.0.0";
		data["width"] = 640;
		data["height"] = 480;
	}

	template<typename T>
	auto& operator [](const T& input) { return data[input]; }

	operator json() {
		return data;
	}
};