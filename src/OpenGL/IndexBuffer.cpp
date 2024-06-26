#include "IndexBuffer.h"
#include "Renderer.h"

namespace OGL
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count)
	{
		GlCall(glGenBuffers(1, &m_RendererID));
		Bind();
		GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GlCall(glDeleteBuffers(1, &m_RendererID));
	}

	void IndexBuffer::Bind() const
	{
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void IndexBuffer::Unbind()
	{
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
