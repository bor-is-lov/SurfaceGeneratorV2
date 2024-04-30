#pragma once

#include <fstream>

#include "Chunk/Chunk.h"

class ChunkLoader
{
	std::ifstream fin;
	std::ofstream fout;

public:
	bool Load(Chunk& chunk, unsigned int seed);

	void Save(const Chunk& chunk, unsigned int seed);
};
