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
		window.StartRender();
		drawer->OnUpdate(0);
		drawer->OnRender();
		drawer->OnGuiRender();
		window.EndRender();
	}

	delete drawer;
	return 0;
}
