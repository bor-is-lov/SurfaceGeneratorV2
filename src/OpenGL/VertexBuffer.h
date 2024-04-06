#pragma once

class VertexBuffer
{
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	static void Unbind();

	void Buffer(const void* data, unsigned int size);
};