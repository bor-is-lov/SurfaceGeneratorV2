#pragma once

#include <string>
#include "Renderer.h"

namespace OGL
{
	class TextureArray
	{
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffers;
		unsigned int m_Width, m_Height, m_Amount;

	public:
		TextureArray(unsigned char* data, const unsigned int amount, const unsigned int width, const unsigned int height);
		~TextureArray();

		void Bind(unsigned int slot = 0) const;
		static void Unbind();

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
	};
}
