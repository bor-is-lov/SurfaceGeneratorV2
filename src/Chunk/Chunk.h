#pragma once

struct Chunk
{
	int x, y;
	short height[16 * 16];
	Chunk() : x(0), y(0)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = 0;
	}
	Chunk(int x, int y) : x(x), y(y)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = 0;
	}
	Chunk(int x, int y, short height[16 * 16]) : x(x), y(y)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = height[i];
	}
};
