#include "TextureArray.h"

#include "stb_image.h"

namespace OGL
{
	TextureArray::TextureArray(const std::string* paths, const unsigned int amount)
		: m_RendererID(0), m_LocalBuffers(nullptr), m_Width(0), m_Height(0), m_BPP(0)
	{
		if (amount < 1)
			ASSERT(false);

		m_FilePaths.clear();
		m_FilePaths.reserve(amount);
		for (size_t i = 0; i < amount; i++)
			m_FilePaths.push_back(paths[i]);

		stbi_set_flip_vertically_on_load(1);

		unsigned char* firstImage = stbi_load(m_FilePaths[0].c_str(), &m_Width, &m_Height, &m_BPP, 4);
		m_LocalBuffers = new unsigned char[m_Width * m_Height * 4 * (amount + 1)];
		for (size_t i = 0; i < m_Width * m_Height; i++)
		{
			m_LocalBuffers[i * 4] = 0;
			m_LocalBuffers[i * 4 + 1] = 0;
			m_LocalBuffers[i * 4 + 2] = 0;
			m_LocalBuffers[i * 4 + 3] = 255;
		}
		for (size_t i = 0; i < m_Width * m_Height * 4; i++)
			m_LocalBuffers[m_Width * m_Height * 4 + i] = firstImage[i];
		stbi_image_free(firstImage);
		for (size_t i = 2; i < amount + 1; i++)
		{
			unsigned char* image = stbi_load(m_FilePaths[i - 1].c_str(), &m_Width, &m_Height, &m_BPP, 4);
			for (size_t j = 0; j < m_Width * m_Height * 4; j++)
				m_LocalBuffers[m_Width * m_Height * i * 4 + j] = image[j];
			stbi_image_free(image);
		}

		GlCall(glGenTextures(1, &m_RendererID));
		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID));

		GlCall(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, m_Width, m_Height, amount + 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffers));

		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

		delete[] m_LocalBuffers;
	}

	TextureArray::~TextureArray()
	{
		GlCall(glDeleteTextures(1, &m_RendererID));
	}

	void TextureArray::Bind(unsigned int slot) const
	{
		GlCall(glActiveTexture(GL_TEXTURE0 + slot));
		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID));
	}

	void TextureArray::Unbind()
	{
		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
	}
}
