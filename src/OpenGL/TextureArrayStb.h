#pragma once

#include <string>
#include "Renderer.h"

namespace OGL
{
	class TextureArrayStb
	{
		unsigned int m_RendererID;
		std::vector<std::string> m_FilePaths;
		unsigned char* m_LocalBuffers;
		int m_Width, m_Height, m_BPP;

	public:
		TextureArrayStb(const std::string* paths, const unsigned int amount);
		~TextureArrayStb();

		void Bind(unsigned int slot = 0) const;
		static void Unbind();

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		inline unsigned int GetBPP() const { return m_BPP; }
	};
}
