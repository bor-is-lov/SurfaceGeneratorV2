#pragma once

struct Chunk
{
	long long x, y;
	short height[16 * 16];
	Chunk() : x(0), y(0)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = 0;
	}
	Chunk(long long x, long long y) : x(x), y(y)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = 0;
	}
	Chunk(long long x, long long y, short height[16 * 16]) : x(x), y(y)
	{
		for (size_t i = 0; i < 16 * 16; i++)
			this->height[i] = height[i];
	}
};
