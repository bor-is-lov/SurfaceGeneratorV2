#include "ChunkGenerator.h"

ChunkGenerator::ChunkGenerator() : m_Seed(0), m_RandomEngine(), m_Distribution(-1023, 1024) {}

ChunkGenerator::ChunkGenerator(unsigned int seed) : m_Seed(seed), m_RandomEngine(), m_Distribution(-1023, 1024) {}

void ChunkGenerator::GenSrc(Chunk& chunk)
{
	unsigned int seed_x, seed_y;
	memcpy(&seed_x, &chunk.x, sizeof(unsigned int));
	memcpy(&seed_y, &chunk.y, sizeof(unsigned int));
	std::seed_seq seed_sequence{m_Seed, seed_x, seed_y};
	m_RandomEngine.seed(seed_sequence);
	for (size_t i = 0; i < 16 * 16; i++)
		chunk.height[i] = m_Distribution(m_RandomEngine);
}
