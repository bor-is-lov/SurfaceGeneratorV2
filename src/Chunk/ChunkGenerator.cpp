#include <random>

#include "ChunkGenerator.h"

ChunkGenerator::ChunkGenerator()
{
	srand(time(NULL));
	m_Seed = rand();
}

ChunkGenerator::ChunkGenerator(unsigned int seed) : m_Seed(seed) {}

void ChunkGenerator::GenSrc(Chunk& chunk)
{
	unsigned int seed_x, seed_y;
	memcpy(&seed_x, &chunk.x, sizeof(unsigned int));
	memcpy(&seed_y, &chunk.y, sizeof(unsigned int));
	std::seed_seq seed_sequence{m_Seed, seed_x, seed_y};
	std::mt19937 random_engine(seed_sequence);
	std::uniform_int_distribution<short> distribution(-1023, 1024); // 0 is a sea level
	for (size_t i = 0; i < 16 * 16; i++)
		chunk.height[i] = distribution(random_engine);
}

