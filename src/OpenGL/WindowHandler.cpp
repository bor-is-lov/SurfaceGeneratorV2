#include "WindowHandler.h"

namespace OGL
{
	WindowHandler::WindowHandler()
		: window(nullptr),
		zoom(1.0f),
		window_max(0),
		renderer(),
		projection(glm::ortho(-1.0f / 9 * 16, 1.0f / 9 * 16, -1.0f, 1.0f, -1.0f, 1.0f)),
		context(nullptr),
		io(nullptr),
		showGui(true)
	{
		window = glfwCreateWindow(960, 540, "Surface Generator V2", NULL, NULL);
		if (!window) {
			std::cout << "GLFW Error: failed to create window\n";
			glfwTerminate();
			ASSERT(false);
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		GlCall(glEnable(GL_BLEND));
		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

		unsigned int err = glewInit();
		if (err != GLEW_OK) {
			std::cout << "GLEW Error: " << glewGetErrorString(err) << '\n';
			ASSERT(false);
		}
		std::cout << glGetString(GL_VERSION) << std::endl;

		IMGUI_CHECKVERSION();
		context = ImGui::CreateContext();

		io = &ImGui::GetIO();
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	WindowHandler::~WindowHandler()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowHandler::StartRender()
	{
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	//void WindowHandler::OnUpdate(float deltaTime)
	//{
	//	// Zoom & window_min set for mouse dragging
	//	int width, height;
	//	glfwGetFramebufferSize(window, &width, &height);
	//	float orthoWidth, orthoHeight;
	//	if (width < height)
	//	{
	//		window_max = height;
	//		orthoHeight = zoom;
	//		orthoWidth = (float)width / height * zoom;
	//	}
	//	else
	//	{
	//		window_max = width;
	//		orthoHeight = (float)height / width * zoom;
	//		orthoWidth = zoom;
	//	}
	//	projection = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0f, 1.0f);
	//}
	//
	//void WindowHandler::OnRender()
	//{
	//	
	//}
	//
	//void WindowHandler::OnGuiRender()
	//{
	//	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	//		showGui = !showGui;
	//
	//	if (showGui)
	//	{
	//		
	//	}
	//}

	void WindowHandler::EndRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
