#pragma once

#include "Chunk.h"

class ChunkGenerator
{
	unsigned int m_Seed;

public:
	ChunkGenerator();
	ChunkGenerator(unsigned int seed);

	void GenSrc(Chunk& chunk);
	// amount of src chunks has to be exactly (ceil(radius / 16.0) * 2 + 1) ^ 2
	bool Gen(unsigned int radius, const Chunk* src[], Chunk& chunk);
};
