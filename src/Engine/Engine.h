#pragma once

#include "Core.h"
#include "Config.h"
#include "Window.h"
#include "Input.h"
#include "Timer.h"
#include "Editor/Editor.h"
#include "Node/Node.h"
#include "Node/CameraNode.h"

struct StaticMeshNode;

struct Engine
{
	Config config;

	Ref<Window> window;

	InputActions input_actions;

	Node* scene_root = new Node("root");
	Ref<Editor> editor;

	Timer window_resize_timer = Timer(0.3333f, true);

	bool exit = false;

	void Run();

	void InitializeLibraries();

	bool OnWindowClose(WindowCloseEvent& e) {
		exit = true;
		std::cout << "closing time\n";
		return true;
	}
	bool OnWindowResize(WindowResizeEvent& e);

	bool OnKeyPressed(KeyPressedEvent& e)
	{
		if(e.GetRepeatCount() == 0)
			input_actions.key_pressed(e.GetKeyCode());
		return false;
	}

	bool OnKeyReleased(KeyReleasedEvent& e)
	{
		input_actions.key_released(e.GetKeyCode());
		return false;
	}

	void OnEvent(Event& e);

	Engine();
	~Engine();
};