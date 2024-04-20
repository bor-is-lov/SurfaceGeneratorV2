#include "ChunkManager.h"

void ChunkManager::GenTextureTest(Chunk& chunk, unsigned char texture[16 * 16 * 4])
{
	if (!m_Loader.LoadSrc(chunk, m_Generator.seed()))
	{
		m_Generator.GenSrc(chunk);
		m_Loader.SaveSrc(chunk, m_Generator.seed());
	}
	for (size_t i = 0; i < 16 * 16; i++)
	{
		texture[i * 4]	   = (1023 + chunk.height[i]) * 255 / 2047; // r
		texture[i * 4 + 1] = (1023 + chunk.height[i]) * 255 / 2047; // g
		texture[i * 4 + 2] = (1023 + chunk.height[i]) * 255 / 2047; // b
		texture[i * 4 + 3] = 255;
	}
}
