#include "Drawer.h"
#include <iostream>

struct ChunkInfo
{
	int x, y;
	unsigned int textureID;
	ChunkInfo() : x(0), y(0), textureID(0) {}; // -1 is no texture (solid black)
	ChunkInfo(int x, int y) : x(x), y(y), textureID(0) {};
};

Drawer::Drawer(GLFWwindow* window)
	: m_Window(window),
	m_io(ImGui::GetIO()),
	m_DrawGUI(false),
	m_Zoom(64.0f),
	m_WindowMax(0),
	m_ViewPos{0.0f, 0.0f},
	m_Boost{0.0f, 0.0f},

	m_Indeces(nullptr),
	m_ChunksInfo(nullptr),
	m_ChunksCenter{0, 0},
	m_RenderDistance(16),
	m_MaxRender(true),
	m_Manager(),

	m_va(),
	m_Layout(),
	m_Shader("res/shaders/shader.vert", "res/shaders/shader.frag"),
	m_Textures(nullptr, 0, 4096, 16, 16),
	m_Projection(glm::ortho(-1.0f / 9 * 16, 1.0f / 9 * 16, -1.0f, 1.0f, -1.0f, 1.0f)),
	m_Renderer()
{
	m_ChunksInfo = new ChunkInfo[CHUNKS_AMOUNT];
	m_Indeces = new unsigned int[INDECES_AMOUNT];
	
	{
		const int width = (int)sqrt(CHUNKS_AMOUNT);
		for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
		{
			m_ChunksInfo[i].x = (int)i % width - width / 2;
			m_ChunksInfo[i].y = (int)i / width - width / 2;
		}
	}

	for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
	{
		m_Buffer[i * 20]	  = m_ChunksInfo[i].x;
		m_Buffer[i * 20 + 1]  = m_ChunksInfo[i].y;
		m_Buffer[i * 20 + 2]  = 0.0f;
		m_Buffer[i * 20 + 3]  = 0.0f;
		m_Buffer[i * 20 + 4]  = m_ChunksInfo[i].textureID = -1;
		m_Buffer[i * 20 + 5]  = m_ChunksInfo[i].x + 1.0f;
		m_Buffer[i * 20 + 6]  = m_ChunksInfo[i].y;
		m_Buffer[i * 20 + 7]  = 1.0f;
		m_Buffer[i * 20 + 8]  = 0.0f;
		m_Buffer[i * 20 + 9]  = m_ChunksInfo[i].textureID;
		m_Buffer[i * 20 + 10] = m_ChunksInfo[i].x + 1.0f;
		m_Buffer[i * 20 + 11] = m_ChunksInfo[i].y + 1.0f;
		m_Buffer[i * 20 + 12] = 1.0f;
		m_Buffer[i * 20 + 13] = 1.0f;
		m_Buffer[i * 20 + 14] = m_ChunksInfo[i].textureID;
		m_Buffer[i * 20 + 15] = m_ChunksInfo[i].x;
		m_Buffer[i * 20 + 16] = m_ChunksInfo[i].y + 1.0f;
		m_Buffer[i * 20 + 17] = 0.0f;
		m_Buffer[i * 20 + 18] = 1.0f;
		m_Buffer[i * 20 + 19] = m_ChunksInfo[i].textureID;

		m_Indeces[i * 6]	 = i * 4;
		m_Indeces[i * 6 + 1] = i * 4 + 1;
		m_Indeces[i * 6 + 2] = i * 4 + 2;
		m_Indeces[i * 6 + 3] = i * 4;
		m_Indeces[i * 6 + 4] = i * 4 + 2;
		m_Indeces[i * 6 + 5] = i * 4 + 3;
	}
	UpdateTextureIDs();

	m_vb = new OGL::VertexBuffer(m_Buffer, sizeof(m_Buffer));
	m_ib = new OGL::IndexBuffer(m_Indeces, INDECES_AMOUNT);

	m_Layout.Push<float>(2, 0);
	m_Layout.Push<float>(2, 0);
	m_Layout.Push<float>(1, 0);
	m_va.AddBuffer(*m_vb, m_Layout);

	m_Textures.Bind();
}

Drawer::~Drawer()
{
	delete[] m_ChunksInfo;
	delete[] m_Indeces;
	delete m_vb;
	delete m_ib;
}

void Drawer::OnUpdate(float deltaTime)
{
	// Zoom & WindowMax set needed for mouse dragging
	{
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
		m_Projection = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0f, 1.0f);
	}

	// update chunks
	if ((int)m_ViewPos[0] - m_ChunksCenter[0] != 0 || (int)m_ViewPos[1] - m_ChunksCenter[1] != 0)
	{
		m_ChunksCenter[0] = (int)m_ViewPos[0];
		m_ChunksCenter[1] = (int)m_ViewPos[1];

		const int width = (int)sqrt(CHUNKS_AMOUNT);
		for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
		{
			bool updateTexture = false;
			if (m_ChunksInfo[i].x < -width / 2 - (int)m_ViewPos[0])
			{
				m_ChunksInfo[i].x += width;
				updateTexture = true;
			}
			if (m_ChunksInfo[i].x > width / 2 - (int)m_ViewPos[0])
			{
				m_ChunksInfo[i].x -= width;
				updateTexture = true;
			}
			if (m_ChunksInfo[i].y < -width / 2 - (int)m_ViewPos[1])
			{
				m_ChunksInfo[i].y += width;
				updateTexture = true;
			}
			if (m_ChunksInfo[i].y > width / 2 - (int)m_ViewPos[1])
			{
				m_ChunksInfo[i].y -= width;
				updateTexture = true;
			}
			if(updateTexture)
				m_ChunksInfo[i].textureID = -1;

			m_Buffer[i * 20]	  =  m_ChunksInfo[i].x;
			m_Buffer[i * 20 + 1]  =  m_ChunksInfo[i].y;
			m_Buffer[i * 20 + 5]  =  m_ChunksInfo[i].x + 1.0f;
			m_Buffer[i * 20 + 6]  =  m_ChunksInfo[i].y;
			m_Buffer[i * 20 + 10] =  m_ChunksInfo[i].x + 1.0f;
			m_Buffer[i * 20 + 11] =  m_ChunksInfo[i].y + 1.0f;
			m_Buffer[i * 20 + 15] =  m_ChunksInfo[i].x;
			m_Buffer[i * 20 + 16] =  m_ChunksInfo[i].y + 1.0f;
		}
		UpdateTextureIDs();
		m_vb->Buffer(m_Buffer, sizeof(m_Buffer));
	}

	// hide/show GUI
	if (ImGui::IsKeyPressed(ImGuiKey_Escape) && !m_io.WantTextInput)
		m_DrawGUI = !m_DrawGUI;

	// move and zoom camera
	{
		if (ImGui::IsKeyDown(ImGuiKey_W) && !m_io.WantTextInput && m_Boost[1] > -0.025f)
			m_Boost[1] -= 0.0001f;
		else if (m_Boost[1] < 0.0f)
		{
			m_Boost[1] += 0.0001f;
			if (m_Boost[1] > 0.0f)
				m_Boost[1] = 0.0f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S) && !m_io.WantTextInput && m_Boost[1] < 0.025f)
			m_Boost[1] += 0.0001f;
		else if (m_Boost[1] > 0.0f)
		{
			m_Boost[1] -= 0.0001f;
			if (m_Boost[1] < 0.0f)
				m_Boost[1] = 0.0f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D) && !m_io.WantTextInput && m_Boost[0] > -0.025f)
			m_Boost[0] -= 0.0001f;
		else if (m_Boost[0] < 0.0f)
		{
			m_Boost[0] += 0.0001f;
			if (m_Boost[0] > 0.0f)
				m_Boost[0] = 0.0f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A) && !m_io.WantTextInput && m_Boost[0] < 0.025f)
			m_Boost[0] += 0.0001f;
		else if (m_Boost[0] > 0.0f)
		{
			m_Boost[0] -= 0.0001f;
			if (m_Boost[0] < 0.0f)
				m_Boost[0] = 0.0f;
		}
		m_ViewPos[0] += m_Boost[0] * m_Zoom;
		m_ViewPos[1] += m_Boost[1] * m_Zoom;
		if (m_io.MouseWheel > 0 && !m_io.WantCaptureMouse)
		{
			m_Zoom *= 0.95f;
			if (m_Zoom < 0.1f)
				m_Zoom = 0.1f;
		}
		if (m_io.MouseWheel < 0 && !m_io.WantCaptureMouse)
		{
			m_Zoom /= 0.95f;
			if (m_Zoom > 30.0f)
				m_Zoom = 30.0f;
		}
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
			m_Zoom = 4.0f;
		if (!m_io.WantCaptureMouse && (ImGui::IsMouseDown(ImGuiMouseButton_Left) || ImGui::IsMouseDown(ImGuiMouseButton_Right)))
		{
			m_Boost[0] = 0.0f;
			m_Boost[1] = 0.0f;
			m_ViewPos[0] += m_io.MouseDelta.x / m_WindowMax * 2 * m_Zoom;
			m_ViewPos[1] -= m_io.MouseDelta.y / m_WindowMax * 2 * m_Zoom;
		}
	}
}

void Drawer::OnRender()
{
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(m_ViewPos[0], m_ViewPos[1], 0.0f));
	glm::mat4 mvp = m_Projection * view;
	m_Shader.SetUniformMat4f("u_MVP", mvp);

	m_Renderer.Draw(m_va, *m_ib, m_Shader);
}

void Drawer::OnGuiRender()
{
	if (m_DrawGUI)
	{
		ImGui::Begin("Settings");
		if (ImGui::CollapsingHeader("Project Info"))
		{
			if (ImGui::TreeNode("About"))
			{
				ImGui::Text(
					"This project is inspired by Minecraft landscape generation.\n" \
					"It generates landscape based on seed in which you can move and teleport.\n" \
					"I was to bored to search for noises realisation, so I've made one myself \n" \
					"(i'm not sure if threre is such a noise =D).\n" \
					"Here's the link of the project's source code:"
				);
				char link[] = "https://github.com/bor-is-lov/SurfaceGeneratorV2";
				ImGui::InputText(" ", link, 49);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Controls"))
			{
				ImGui::Text(
					"Esc - open/close this window\n" \
					"WASD, Mouse dragging - move camera\n" \
					"Mouse wheel - zoom\n" \
					"Mouse wheel click - reset zoom\n"
				);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Links"))
			{
				char Chernos[]	= "https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2",
					glm[]		= "https://github.com/g-truc/glm",
					ImGui[]		= "https://github.com/ocornut/imgui",
					stb_image[]	= "https://github.com/nothings/stb/blob/master/stb_image.h";
				ImGui::Text("This project is made with help of:");
				ImGui::Text("TheCherno's OpenGL tutorials:");
				ImGui::InputText(" ", Chernos, 73);
				ImGui::Text("GLM:");
				ImGui::InputText(" ", glm, 30);
				ImGui::Text("ImGui:");
				ImGui::InputText(" ", ImGui, 33);
				ImGui::Text("stb_image.h (i'm not sure if i'll use it):");
				ImGui::InputText(" ", stb_image, 56);

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		static unsigned int seed = 0;
		if (ImGui::InputInt("Seed", (int*)&seed))
		{
			m_Manager.SetSeed(seed);
			for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
				m_ChunksInfo[i].textureID = -1;
			UpdateTextureIDs();
		}

		char x[10], y[10];
		_itoa_s(-(int)(m_ViewPos[0] * 16), x, 10, 10);
		_itoa_s(-(int)(m_ViewPos[1] * 16), y, 10, 10);
		ImGui::Text("Pos: ");
		ImGui::SameLine();
		ImGui::Text(x);
		ImGui::SameLine();
		ImGui::Text(y);

		static int tp[] = {0, 0};
		if (ImGui::Button("tp to"))
		{
			m_Boost[0]	 = 0.0f;
			m_Boost[1]	 = 0.0f;
			m_ViewPos[0] = -tp[0] / 16.0f;
			m_ViewPos[1] = -tp[1] / 16.0f;

			const int width = (int)sqrt(CHUNKS_AMOUNT);
			for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
			{
				m_ChunksInfo[i].x = (int)i % width - width / 2 - (int)m_ViewPos[0];
				m_ChunksInfo[i].y = (int)i / width - width / 2 - (int)m_ViewPos[1];
				m_ChunksInfo[i].textureID = -1;
			}
		}
		ImGui::SameLine();
		ImGui::InputInt2(" ", tp);

		if (ImGui::SliderInt("Render distance", (int*)&m_RenderDistance, 0, 48))
		{
			UpdateTextureIDs();
			m_vb->Buffer(m_Buffer, sizeof(m_Buffer));
		}

		if (ImGui::Checkbox("Maximum render distance", &m_MaxRender))
		{
			UpdateTextureIDs();
			m_vb->Buffer(m_Buffer, sizeof(m_Buffer));
		}

		ImGui::Separator();

		ImGui::Text("%.1f FPS", m_io.Framerate);
		ImGui::End();
	}
}

void Drawer::UpdateTextureIDs()
{
	for (size_t i = 0; i < CHUNKS_AMOUNT; i++)
	{
		bool updateTexture = (m_ChunksInfo[i].textureID == -1);
		if (m_MaxRender || (m_ChunksInfo[i].x + m_ViewPos[0]) * (m_ChunksInfo[i].x + m_ViewPos[0]) + (m_ChunksInfo[i].y + m_ViewPos[1]) * (m_ChunksInfo[i].y + m_ViewPos[1]) <= m_RenderDistance * m_RenderDistance)
			m_Buffer[i * 20 + 4] =
			m_Buffer[i * 20 + 9] =
			m_Buffer[i * 20 + 14] =
			m_Buffer[i * 20 + 19] =
			m_ChunksInfo[i].textureID = i;
		else
		{
			m_Buffer[i * 20 + 4] =
			m_Buffer[i * 20 + 9] =
			m_Buffer[i * 20 + 14] =
			m_Buffer[i * 20 + 19] =
			m_ChunksInfo[i].textureID = -1;
			updateTexture = false;
		}
		if (updateTexture)
		{
			unsigned char texture[16 * 16 * 4];
			Chunk chunk(m_ChunksInfo[i].x, m_ChunksInfo[i].y);
			m_Manager.GenTextureTest(chunk, texture);
			m_Textures.SubImage(texture, i);
		}
	}
}
