#pragma once

#include "VertexBuffer.h"
class BufferLayout;

class VertexArray
{
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	static void Unbind();
	void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout);
};