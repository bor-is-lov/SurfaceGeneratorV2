#include "ChunkManager.h"

void ChunkManager::ChunkToTexTest(Chunk& chunk, unsigned char texture[16 * 16 * 4])
{
	for (size_t i = 0; i < 16*16; i++)
	{
		texture[i * 4] = chunk.x;
		texture[i * 4] = chunk.y;
		texture[i * 4] = 0;
		texture[i * 4] = 255;
	}
}
