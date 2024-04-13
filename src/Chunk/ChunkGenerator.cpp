#include <random>

#include "ChunkGenerator.h"

ChunkGenerator::ChunkGenerator()
{
	srand(time(NULL));
	m_Seed = rand();
}
