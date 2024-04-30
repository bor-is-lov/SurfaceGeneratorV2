#pragma once

#include <random>

#include "Chunk.h"

class ChunkGenerator
{
	unsigned int m_Seed;
	std::mt19937 m_RandomEngine;
	std::uniform_int_distribution<short> m_Distribution;

public:
	ChunkGenerator();
	ChunkGenerator(unsigned int seed);

	inline unsigned int GetSeed() { return m_Seed; }

	void GenSrc(Chunk& chunk);
	void Gen(unsigned int radius, Chunk& chunk);

	inline unsigned int seed() { return m_Seed; }
};
