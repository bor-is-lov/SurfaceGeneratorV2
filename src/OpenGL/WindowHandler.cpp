#include "WindowHandler.h"

namespace OGL
{
	WindowHandler::WindowHandler(const std::string& name)
		: m_Window(nullptr),
		m_Renderer(),
		m_context(nullptr),
		m_io(nullptr)
	{
		m_Window = glfwCreateWindow(960, 540, name.c_str(), NULL, NULL);
		if (!m_Window) {
			std::cout << "GLFW Error: failed to create window\n";
			glfwTerminate();
			ASSERT(false);
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(0);

		GlCall(glEnable(GL_BLEND));
		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

		unsigned int err = glewInit();
		if (err != GLEW_OK) {
			std::cout << "GLEW Error: " << glewGetErrorString(err) << '\n';
			ASSERT(false);
		}

		IMGUI_CHECKVERSION();
		m_context = ImGui::CreateContext();

		m_io = &ImGui::GetIO();
		m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	WindowHandler::~WindowHandler()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowHandler::StartRender()
	{
		m_Renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void WindowHandler::EndRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
