#include "ChunkGenerator.h"
#include <mutex>

std::mutex mutex;

ChunkGenerator::ChunkGenerator() : m_Seed(0), m_RandomEngine(), m_Distribution(-1023, 1024) {}

ChunkGenerator::ChunkGenerator(unsigned int seed) : m_Seed(seed), m_RandomEngine(), m_Distribution(-1023, 1024) {}

void ChunkGenerator::GenSrc(Chunk& chunk)
{
	unsigned int seed_x, seed_y;
	memcpy(&seed_x, &chunk.x, sizeof(unsigned int));
	memcpy(&seed_y, &chunk.y, sizeof(unsigned int));
	mutex.lock();
	std::seed_seq seed_sequence{m_Seed, seed_x, seed_y};
	m_RandomEngine.seed(seed_sequence);
	for (size_t i = 0; i < 16 * 16; i++)
		chunk.height[i] = m_Distribution(m_RandomEngine);
	mutex.unlock();
}

void ChunkGenerator::Gen(unsigned int radius, Chunk& chunk)
{
	const int CHUNKS_RADIUS = ceil(radius / 16.0);
	const int CHUNKS_AMOUNT = (CHUNKS_RADIUS * 2 + 1) * (CHUNKS_RADIUS * 2 + 1);
	Chunk* srcChunks = new Chunk[CHUNKS_AMOUNT];

	{
		int i = 0;
		for (int y = chunk.y - CHUNKS_RADIUS; y < chunk.y + CHUNKS_RADIUS + 1; y++)
		{
			for (int x = chunk.x - CHUNKS_RADIUS; x < chunk.x + CHUNKS_RADIUS + 1; x++)
			{
				srcChunks[i].x = x;
				srcChunks[i].y = y;
				GenSrc(srcChunks[i]);
				i++;
			}
		}
	}

	for (size_t i = 0; i < 16*16; i++)
	{
		int x = chunk.x * 16 + i % 16;
		int y = chunk.y * 16 + i / 16;
		int sum = 0;
		int amount = 0;

		for (size_t srcChunks_i = 0; srcChunks_i < CHUNKS_AMOUNT; srcChunks_i++)
		{
			for (size_t j = 0; j < 16*16; j++)
			{
				int src_x = srcChunks[srcChunks_i].x * 16 + j % 16;
				int src_y = srcChunks[srcChunks_i].y * 16 + j / 16;

				double influence = 1 - sqrt((x - src_x) * (x - src_x) + (y - src_y) * (y - src_y)) / radius;
				if (influence > 0.0)
					sum += srcChunks[srcChunks_i].height[j] * influence;
				amount++;
			}
		}
		sum *= 40 * sqrt(radius);
		chunk.height[i] = sum / amount;
		if (chunk.height[i] < -1023)
			chunk.height[i] = -1023;
		if (chunk.height[i] > 1024)
			chunk.height[i] = 1024;
	}

	delete[] srcChunks;
}
