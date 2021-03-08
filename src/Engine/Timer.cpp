#include "Timer.h"

#define GLFW_INLCUDE_NONE

#include "GLFW/glfw3.h"

Timer::Timer(f32 _t, bool _oneshot) : time(glfwGetTime()), tick(_t), oneshot(_oneshot)
{
}

f32 Timer::GetDeltaTime()
{
	if (!active) return 0;
	return glfwGetTime() - time;
}

void Timer::Start()
{
	active = true;
	time = glfwGetTime();
}

bool Timer::Tick()
{
	if (GetDeltaTime() > tick && active)
	{
		if (oneshot)
		{
			active = false;
			return true;
		}
		Start();
		return true;
	}
	return false;
}
