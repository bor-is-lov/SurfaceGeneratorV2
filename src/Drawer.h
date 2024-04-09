#pragma once

#include "vendor/ImGUI/imgui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGL/BufferLayout.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/TextureArray.h"

#include "Chunk/Chunk.h"

class Drawer
{
	GLFWwindow* m_Window;
	ImGuiIO& m_io;
	bool m_DrawGUI;
	float m_Zoom;
	int m_WindowMax;
	float m_ViewPos[2];
	float m_Boost[2];

	static const unsigned int CHUNKS_AMOUNT = 64*64,
		POSITIONS_AMOUNT = 20 * CHUNKS_AMOUNT,
		INDECES_AMOUNT   = 6  * CHUNKS_AMOUNT,
		MODEL_AMOUNT	 = 2  * CHUNKS_AMOUNT;
	float m_Buffer[POSITIONS_AMOUNT];
	unsigned int m_Indeces[INDECES_AMOUNT];
	Chunk* m_Chunks;

	OGL::VertexArray m_va;
	OGL::VertexBuffer* m_vb;
	OGL::BufferLayout m_Layout;
	OGL::IndexBuffer* m_ib;
	OGL::Shader m_Shader;
	OGL::TextureArray* m_Textures;
	glm::mat4 m_Projection;
	OGL::Renderer m_Renderer;
public:
	Drawer(GLFWwindow* window);
	~Drawer();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnGuiRender();
};