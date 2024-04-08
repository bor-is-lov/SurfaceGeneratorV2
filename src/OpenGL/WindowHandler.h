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
		GLFWwindow* m_Window;
		Renderer m_Renderer;
		ImGuiContext* m_context;
		ImGuiIO* m_io;

	public:
		WindowHandler(const std::string& name);
		~WindowHandler();

		inline bool ShouldClose() { return glfwWindowShouldClose(m_Window); }
		inline GLFWwindow* window() { return m_Window; }
		inline Renderer& renderer() { return m_Renderer; }
		inline ImGuiIO* io() { return m_io; }

		void StartRender();
		void EndRender();
	};
}
