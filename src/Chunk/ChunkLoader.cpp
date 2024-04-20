#include <string>
#include <filesystem>
#include "ChunkLoader.h"

static const char* SAVESPATH = "saves/";

bool ChunkLoader::LoadSrc(Chunk& chunk, unsigned int seed)
{
	fin.open(SAVESPATH + std::to_string(seed) + "/src_" + std::to_string(chunk.x) + "_" + std::to_string(chunk.y) + ".txt");
	if(!fin.is_open())
	    return false;

	std::string num;
	int i = 0;
	while (!fin.eof())
	{
		char a = fin.get();
		if (a == '\t' || a == '\n')
		{
			chunk.height[i] = atoi(num.c_str());
			i++;
			num.clear();
		}
		else
			num += a;
	}

	fin.close();
	return true;
}

void ChunkLoader::SaveSrc(const Chunk& chunk, unsigned int seed)
{
	std::string path(SAVESPATH + std::to_string(seed) + "/src_" + std::to_string(chunk.x) + "_" + std::to_string(chunk.y) + ".txt");
	fout.open(path);
	if (!fout.is_open())
	{
		std::filesystem::create_directory(SAVESPATH);
		std::filesystem::create_directory(SAVESPATH + std::to_string(seed));
		fout.open(path);
		if (!fout.is_open()) __debugbreak();
	}

	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < 15; j++)
			fout << chunk.height[i * 16 + j] << '\t';
		fout << chunk.height[i * 16 + 15] << '\n';
	}

	fout.close();
}
