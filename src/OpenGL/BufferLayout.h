#pragma once

#include <vector>
#include "Renderer.h"

namespace OGL
{
	struct BufferElement
	{
		unsigned int count;
		unsigned int type;
		unsigned char normalized;

		BufferElement(unsigned int count, unsigned int type, unsigned char normalized)
			: count(count), type(type), normalized(normalized) {};

		static unsigned int GetTypeSize(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:
				return sizeof(float);
			case GL_UNSIGNED_INT:
				return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:
				return sizeof(unsigned char);
			}
			ASSERT(false);
			return 0;
		}

		unsigned int GetTypeSize() const
		{
			switch (type)
			{
			case GL_FLOAT:
				return sizeof(float);
			case GL_UNSIGNED_INT:
				return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:
				return sizeof(unsigned char);
			}
			ASSERT(false);
			return 0;
		}
	};

	class BufferLayout
	{
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride;

	public:
		BufferLayout()
			: m_Stride(0)
		{
		};

		template<typename T>
		void Push(unsigned int count, bool normilized)
		{
			//static_assert(false);
			ASSERT(false);
		}

		template<>
		void Push<float>(unsigned int count, bool normilized)
		{
			m_Elements.emplace_back(count, GL_FLOAT, normilized ? GL_TRUE : GL_FALSE);
			m_Stride += count * sizeof(float);
		}

		template<>
		void Push<unsigned int>(unsigned int count, bool normilized)
		{
			m_Elements.emplace_back(count, GL_UNSIGNED_INT, normilized ? GL_TRUE : GL_FALSE);
			m_Stride += count * sizeof(unsigned int);
		}

		template<>
		void Push<unsigned char>(unsigned int count, bool normilized)
		{
			m_Elements.emplace_back(count, GL_UNSIGNED_BYTE, normilized ? GL_TRUE : GL_FALSE);
			m_Stride += count * sizeof(unsigned char);
		}

		inline std::vector<BufferElement> GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};
}
