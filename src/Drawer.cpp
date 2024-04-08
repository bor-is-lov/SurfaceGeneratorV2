#include "Drawer.h"
#include <iostream>

Drawer::Drawer(GLFWwindow* window)
	: m_Window(window),
	m_io(ImGui::GetIO()),
	m_Zoom(1.0f),
	m_WindowMax(0),
	model
	{
		-1.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	},
	va(),
	layout(),
	shader("res/shaders/shader.vert", "res/shaders/shader.frag"),
	projection(glm::ortho(-1.0f / 9 * 16, 1.0f / 9 * 16, -1.0f, 1.0f, -1.0f, 1.0f)),
	renderer(),
	view_pos{0.0f, 0.0f}
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
	for (size_t i = 0; i < QUADS_AMOUNT; i++)
	{
		positions[i * 20]	   = -0.4f + model[2 * i];
		positions[i * 20 + 1]  = -0.4f + model[2 * i + 1];
		positions[i * 20 + 2]  =  0.0f;
		positions[i * 20 + 3]  =  0.0f;
		positions[i * 20 + 4]  =  0;
		positions[i * 20 + 5]  =  0.4f + model[2 * i];
		positions[i * 20 + 6]  = -0.4f + model[2 * i + 1];
		positions[i * 20 + 7]  =  1.0f;
		positions[i * 20 + 8]  =  0.0f;
		positions[i * 20 + 9]  =  0;
		positions[i * 20 + 10] =  0.4f + model[2 * i];
		positions[i * 20 + 11] =  0.4f + model[2 * i + 1];
		positions[i * 20 + 12] =  1.0f;
		positions[i * 20 + 13] =  1.0f;
		positions[i * 20 + 14] =  0;
		positions[i * 20 + 15] = -0.4f + model[2 * i];
		positions[i * 20 + 16] =  0.4f + model[2 * i + 1];
		positions[i * 20 + 17] =  0.0f;
		positions[i * 20 + 18] =  1.0f;
		positions[i * 20 + 19] =  0;

		indeces[i * 6]	   = i * 4;
		indeces[i * 6 + 1] = i * 4 + 1;
		indeces[i * 6 + 2] = i * 4 + 2;
		indeces[i * 6 + 3] = i * 4;
		indeces[i * 6 + 4] = i * 4 + 2;
		indeces[i * 6 + 5] = i * 4 + 3;
	}

	vb = new OGL::VertexBuffer(positions, sizeof(positions));
	ib = new OGL::IndexBuffer(indeces, INDECES_AMOUNT);

	layout.Push<float>(2, 0);
	layout.Push<float>(2, 0);
	layout.Push<float>(1, 0);
	va.AddBuffer(*vb, layout);

	textures = new OGL::TextureArray(nullptr, 0);
	textures->Bind();
}
Drawer::~Drawer()
{
	delete vb;
	delete ib;
	m_Zoom = 1;
}
void Drawer::OnUpdate(float deltaTime)
{
	// Zoom & window_min set for mouse dragging
	int width, height;
	glfwGetFramebufferSize(m_Window, &width, &height);
	float orthoWidth, orthoHeight;
	if (width < height)
	{
		m_WindowMax = height;
		orthoHeight = m_Zoom;
		orthoWidth = (float)width / height * m_Zoom;
	}
	else
	{
		m_WindowMax = width;
		orthoHeight = (float)height / width * m_Zoom;
		orthoWidth = m_Zoom;
	}
	projection = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0f, 1.0f);

	// move and zoom camera
	if (ImGui::IsKeyDown(ImGuiKey_W))
		view_pos[1] -= 0.01;
	if (ImGui::IsKeyDown(ImGuiKey_S))
		view_pos[1] += 0.01;
	if (ImGui::IsKeyDown(ImGuiKey_D))
		view_pos[0] -= 0.01;
	if (ImGui::IsKeyDown(ImGuiKey_A))
		view_pos[0] += 0.01;
	if (m_io.MouseWheel > 0 && m_Zoom > 0.15f)
		m_Zoom -= 0.1f;
	if (m_io.MouseWheel < 0)
		m_Zoom += 0.1f;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
		m_Zoom = 1;
	if (!m_io.WantCaptureMouse && (ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right)))
	{
		view_pos[0] += m_io.MouseDelta.x / m_WindowMax * 2 * m_Zoom;
		view_pos[1] -= m_io.MouseDelta.y / m_WindowMax * 2 * m_Zoom;
	}
}
void Drawer::OnRender()
{
	for (size_t i = 0; i < QUADS_AMOUNT; i++)
	{
		positions[i * 20]	   = -0.4f + model[2 * i];
		positions[i * 20 + 1]  = -0.4f + model[2 * i + 1];
		positions[i * 20 + 5]  =  0.4f + model[2 * i];
		positions[i * 20 + 6]  = -0.4f + model[2 * i + 1];
		positions[i * 20 + 10] =  0.4f + model[2 * i];
		positions[i * 20 + 11] =  0.4f + model[2 * i + 1];
		positions[i * 20 + 15] = -0.4f + model[2 * i];
		positions[i * 20 + 16] =  0.4f + model[2 * i + 1];
	}
	vb->Buffer(positions, sizeof(positions));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(view_pos[0], view_pos[1], 0.0f));
	glm::mat4 mvp = projection * view;
	shader.SetUniformMat4f("u_MVP", mvp);

	renderer.Draw(va, *ib, shader);
}
void Drawer::OnGuiRender()
{
	//Objects ImGui MVP settings
	{
		ImGui::Begin("Batch Rendering & texture array Settings");
		for (size_t i = 0; i < QUADS_AMOUNT; i++)
		{
			char n[2];
			_itoa_s(i, n, 2, 10);
			ImGui::SliderFloat2((std::string("model") + std::string(n)).c_str(), model + i * 2, -1.0f, 1.0f);
		}
		ImGui::SliderFloat2("view", view_pos, -1.0f, 1.0f);
		if (ImGui::Button("Reset"))
		{
			model[0]	= -1.0f;
			model[1]	=  0.0f;
			model[2]	=  0.0f;
			model[3]	=  0.0f;
			model[4]	=  1.0f;
			model[5]	=  0.0f;
			view_pos[0]	=  0.0f;
			view_pos[1]	=  0.0f;
			m_Zoom = 1;
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
		ImGui::End();
	}
}
