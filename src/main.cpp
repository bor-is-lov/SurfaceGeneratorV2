#include "OpenGL/WindowHandler.h"

int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Error: failed to initialize the library\n";
		return -1;
	}
	OGL::WindowHandler window("Surface Generator V2");

	while (!window.ShouldClose())
	{
		window.StartRender();
		window.EndRender();
	}

	return 0;
}
