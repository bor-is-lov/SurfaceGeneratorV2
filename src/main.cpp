#include <chrono>

#include "OpenGL/WindowHandler.h"
#include "Drawer.h"

int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Error: failed to initialize the library\n";
		return -1;
	}
	OGL::WindowHandler window("Surface Generator V2");
	Drawer* drawer = new Drawer(window.window());

	while (!window.ShouldClose())
	{
		static float delta = 0.0f;
		auto startFrame = std::chrono::system_clock::now();
		window.StartRender();
		drawer->OnUpdate(delta);
		drawer->OnRender();
		drawer->OnGuiRender();
		window.EndRender();
		auto endFrame = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaTime = endFrame - startFrame;
		delta = deltaTime.count();
	}

	delete drawer;
	return 0;
}
