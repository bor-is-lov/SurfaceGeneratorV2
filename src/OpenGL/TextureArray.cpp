#include "TextureArray.h"

namespace OGL
{
	TextureArray::TextureArray(unsigned char* data, const unsigned int amount, const unsigned int reserve, const unsigned int width, const unsigned int height)
		: m_RendererID(0), m_LocalBuffers(nullptr), m_Width(width), m_Height(height), m_Amount(amount), m_Reserved(reserve)
	{
		if (reserve <= amount)
			ASSERT(false);

		m_LocalBuffers = new unsigned char[m_Width * m_Height * m_Reserved * 4];

		for (size_t i = 0; i < m_Width * m_Height; i++)
		{
			if (i % m_Width < m_Width / 2 && i < m_Width * m_Height / 2 || i % m_Width >= m_Width / 2 && i > m_Width * m_Height / 2)
			{
				m_LocalBuffers[i * 4]	  = 0; //r
				m_LocalBuffers[i * 4 + 2] = 0; //b
			}
			else
			{
				m_LocalBuffers[i * 4]	  = 255; //r
				m_LocalBuffers[i * 4 + 2] = 255; //b
			}
			m_LocalBuffers[i * 4 + 1] = 0; //g
			m_LocalBuffers[i * 4 + 3] = 255; //a
		}

		for (size_t i = 0; i < m_Width * m_Height * m_Amount * 4; i++)
			m_LocalBuffers[i + m_Width * m_Height * 4] = data[i];
		
		GlCall(glGenTextures(1, &m_RendererID));
		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID));

		GlCall(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, m_Width, m_Height * 2, ceil(m_Reserved / 2.0)));
		GlCall(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_Width, m_Height * 2, ceil(m_Reserved / 2.0), GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffers));

		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); // want it to be sharp
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
	}

	TextureArray::~TextureArray()
	{
		delete[] m_LocalBuffers;
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

	void TextureArray::SubImage(unsigned char* data, int layerOffset)
	{
		Bind();
		for (size_t i = 0; i < m_Height * m_Width * 4; i++)
			m_LocalBuffers[i + m_Height * m_Width * 4 * layerOffset] = data[i];
		if (layerOffset % 2 == 0)
		{
			GlCall(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layerOffset / 2, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, &m_LocalBuffers[m_Height * m_Width * 4 * layerOffset]));
		}
		else
		{
			GlCall(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, m_Height, layerOffset / 2, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, &m_LocalBuffers[m_Height * m_Width * 4 * layerOffset]));
		}
	}
}
