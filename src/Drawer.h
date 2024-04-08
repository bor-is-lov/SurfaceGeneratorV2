#pragma once

#include "vendor/ImGUI/imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGL/BufferLayout.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/TextureArray.h"


class Drawer
{
	GLFWwindow* m_Window;
	ImGuiIO& m_io;
	float m_Zoom;
	int m_WindowMax;

	static const unsigned int QUADS_AMOUNT = 3,
		POSITIONS_AMOUNT = 20 * QUADS_AMOUNT,
		INDECES_AMOUNT   = 6  * QUADS_AMOUNT,
		MODEL_AMOUNT	 = 2 * QUADS_AMOUNT;
	float positions[POSITIONS_AMOUNT], model[MODEL_AMOUNT];
	unsigned int indeces[INDECES_AMOUNT];
	OGL::VertexArray va;
	OGL::VertexBuffer* vb;
	OGL::BufferLayout layout;
	OGL::IndexBuffer* ib;
	OGL::Shader shader;
	OGL::TextureArray* textures;
	glm::mat4 projection;
	OGL::Renderer renderer;
	float view_pos[2];
public:
	Drawer(GLFWwindow* window);
	~Drawer();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnGuiRender();
};