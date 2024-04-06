#include "Renderer.h"
#include "VertexArray.h"
#include "BufferLayout.h"

namespace OGL
{
	VertexArray::VertexArray()
	{
		GlCall(glGenVertexArrays(1, &m_RendererID));
	}

	VertexArray::~VertexArray()
	{
		GlCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::Bind() const
	{
		GlCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind()
	{
		GlCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (size_t i = 0; i < elements.size(); i++)
		{
			GlCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*)offset));
			GlCall(glEnableVertexAttribArray(i));
			offset += elements[i].count * elements[i].GetTypeSize();
		}
	}
}
