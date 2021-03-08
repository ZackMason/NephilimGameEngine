#pragma once

#include "Core.h"

struct Timer
{
	Timer(f32 _t, bool oneshot = false);
	virtual ~Timer() = default;

	f32 GetDeltaTime();
	void Start();
	bool Tick();

	bool oneshot = false;
	bool active = true;
	f32 time;
	f32 tick;
};