#include "ChunkManager.h"

void ChunkManager::GenTextureTest(Chunk& chunk, unsigned char texture[16 * 16 * 4])
{
	for (size_t i = 0; i < 16 * 16; i++)
	{
		texture[i * 4]	   = chunk.x % 10 * 255 / 10;
		texture[i * 4 + 1] = chunk.y % 10 * 255 / 10;
		texture[i * 4 + 2] = 0;
		texture[i * 4 + 3] = 255;
	}
}
