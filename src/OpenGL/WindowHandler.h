#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "BufferLayout.h"
#include "TextureArray.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

namespace OGL
{
	class WindowHandler
	{
		GLFWwindow* window;
		float zoom;
		int window_max;
		OGL::Renderer renderer;
		glm::mat4 projection;
		ImGuiContext* context;
		ImGuiIO* io;
		bool showGui;

	public:
		WindowHandler();
		~WindowHandler();

		inline bool ShouldClose() { return glfwWindowShouldClose(window); }

		void StartRender();
		void EndRender();
	};
}
