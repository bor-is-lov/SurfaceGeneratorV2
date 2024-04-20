#pragma once

#include <fstream>

#include "Chunk/Chunk.h"

class ChunkLoader
{
	std::ifstream fin;
	std::ofstream fout;

public:
	bool LoadSrc(Chunk& chunk, unsigned int seed);
	bool Load(Chunk& chunk, unsigned int seed);

	void SaveSrc(const Chunk& chunk, unsigned int seed);
	void Save(const Chunk& chunk, unsigned int seed);
};
