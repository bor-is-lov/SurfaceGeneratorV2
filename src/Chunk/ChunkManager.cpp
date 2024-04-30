#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
	m_Generator = new ChunkGenerator;
}

ChunkManager::~ChunkManager()
{
	delete m_Generator;
}

void ChunkManager::SetSeed(unsigned int seed)
{
	delete m_Generator;
	m_Generator = new ChunkGenerator(seed);
}

void ChunkManager::GenTextureTest(Chunk& chunk, unsigned char texture[16 * 16 * 4])
{
	m_Generator->GenSrc(chunk);
	for (size_t i = 0; i < 16 * 16; i++)
	{
		texture[i * 4]	   = (1023 + chunk.height[i]) * 255 / 2047; // r
		texture[i * 4 + 1] = (1023 + chunk.height[i]) * 255 / 2047; // g
		texture[i * 4 + 2] = (1023 + chunk.height[i]) * 255 / 2047; // b
		texture[i * 4 + 3] = 255;
	}
}
