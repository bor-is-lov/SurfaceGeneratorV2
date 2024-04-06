#pragma once

class IndexBuffer
{
	unsigned int m_RendererID, m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	static void Unbind();

	inline unsigned int GetCount() const { return m_Count; }
};