#pragma once
#include "ChunkGenerator.h"
#include "ChunkLoader.h"

class ChunkManager
{
	ChunkGenerator* m_Generator;
	ChunkLoader m_Loader;
public:
	ChunkManager();
	~ChunkManager();

	//don't forget to update textures!
	void SetSeed(unsigned int seed);

	bool GenChunk(Chunk& chunk);
	//texture is an arary of [16*16*4] or [1024] unsigned chars
	void GenTexture(Chunk& chunk, unsigned char texture[16 * 16 * 4]);
	void GenTextureTest(Chunk& chunk, unsigned char texture[16 * 16 * 4]);
};
