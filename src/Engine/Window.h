#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Core.h"
#include "Event.h"

struct Window
{
	GLFWwindow* window;

	void Swap() const;
	void BindCallbacks();
	void Resize(u32 w, u32 h) {}


	Window(u32 w, u32 h, const std::string& name);

	using EventCallbackFn = std::function<void(Event&)>;
	struct WindowData
	{
		WindowData(u32 w, u32 h, const std::string& name) : width(w), height(h), title(name) {}

		std::string title;
		u32 width, height;
		bool vsync = true;

		EventCallbackFn EventCallback;
	} data;
	void SetEventCallback(const EventCallbackFn& callback) { data.EventCallback = callback; }
};