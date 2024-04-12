#pragma once
#include "ChunkGenerator.h"

class ChunkManager
{
	ChunkGenerator m_Generator;
public:
	ChunkManager() {}
	ChunkManager(unsigned int seed) : m_Generator(seed) {}
	bool SetChunk(Chunk& chunk);
	//texture is arary of [16*16*4] or [1024] chars
	void ChunkToTex(Chunk& chunk, unsigned char texture[16 * 16 * 4]);
	void ChunkToTexTest(Chunk& chunk, unsigned char texture[16 * 16 * 4]);
};
