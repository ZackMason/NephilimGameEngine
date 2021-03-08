#pragma once

#include <bitset>
#include <unordered_set>

#include "Core.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

struct InputActions
{
	v2 mouse_position;
	v2 global_mouse_position;
	std::unordered_set<std::string> action_just_pressed;
	std::unordered_set<std::string> action_just_released;
	std::unordered_map<std::string, int> action_pressed;
	std::unordered_map<std::string, std::vector<int>> actions;
	std::unordered_map<int, std::vector<std::string>> keys;
	std::bitset<12> buttons;
	std::bitset<12> single_buttons;

	bool is_action_just_pressed(const std::string& action)
	{
		return action_just_pressed.find(action) != action_just_pressed.end();
	}

	bool is_action_just_released(const std::string& action)
	{
		return action_just_released.find(action) != action_just_released.end();
	}

	void reset_action_state()
	{
		action_just_pressed.clear();
		action_just_released.clear();
		single_buttons.reset();
	}

	bool is_action_pressed(const std::string& action)
	{
		return action_pressed[action] > 0;
	}

	void press_action(const std::string& action_name)
	{
		action_just_pressed.insert(action_name);
		action_pressed[action_name]++;
	}

	void release_action(const std::string& action_name)
	{
		action_just_released.insert(action_name);
		action_pressed[action_name]--;
	}

	void key_pressed(int key)
	{
		auto actions = keys[key];
		for (const auto& action : actions)
			press_action(action);
	}

	void key_released(int key)
	{
		auto actions = keys[key];
		for (const auto& action : actions)
			release_action(action);
	}

	void add_action(const std::string& action_name, int key)
	{
		actions.try_emplace(action_name, std::vector<int>{});
		keys.try_emplace(key, std::vector<std::string>{});
		action_pressed.try_emplace(action_name, 0);

		actions[action_name].push_back(key);
		keys[key].push_back(action_name);
	}
};