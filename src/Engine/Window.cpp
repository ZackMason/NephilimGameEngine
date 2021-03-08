#include "Window.h"

#include <iostream>

void Window::Swap() const
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::BindCallbacks()
{
	glfwSetWindowUserPointer(window, &data);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		data.width = width;
		data.height = height;
		glViewport(0, 0, data.width, data.height);

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
		}

	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		KeyTypedEvent event(keycode);
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.EventCallback(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
}

Window::Window(u32 w, u32 h, const std::string& name) : data(w, h, name)
{
	window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);

	if (!window)
	{
		std::cout << "Error creating window, GLFW Failed\n";
		glfwTerminate();
	}
	else
	{
		glfwMakeContextCurrent(window);
		BindCallbacks();
	}
}
