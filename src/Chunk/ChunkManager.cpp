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

void ChunkManager::GenChunk(Chunk& chunk)
{
	Chunk chunk1(chunk.x, chunk.y), chunk2(chunk.x, chunk.y);
	m_Generator->Gen(90, chunk1);
	m_Generator->Gen(10, chunk2);
	for (size_t i = 0; i < 256; i++)
		chunk.height[i] = 0.9 * chunk1.height[i] + 0.1 * chunk2.height[i];
}

void ChunkManager::GenTexture(Chunk& chunk, unsigned char texture[16 * 16 * 4])
{
	for (size_t i = 0; i < 16 * 16; i++)
	{
		if (chunk.height[i] <= 0)
		{
			texture[i * 4]	   = 0; // r
			texture[i * 4 + 1] = 0; // g
			texture[i * 4 + 2] = (1023 + chunk.height[i]) * 255 / 2048 + 32; // b
		}
		else if (chunk.height[i] < 128)
		{
			texture[i * 4]	   = 160; // r
			texture[i * 4 + 1] = 160; // g
			texture[i * 4 + 2] = chunk.height[i] * 255 / 512; // b
		}
		else
		{
			texture[i * 4]	   = chunk.height[i] * 255 / 2048; // r
			texture[i * 4 + 1] = 160; // g
			texture[i * 4 + 2] = chunk.height[i] * 255 / 2048; // b
		}
		texture[i * 4 + 3] = 255;
	}
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

