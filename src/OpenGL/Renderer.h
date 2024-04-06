#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GlCall(x) OGL::GlClearErrors();\
	x;\
	ASSERT(OGL::GlLogCall(#x, __FILE__, __LINE__))

namespace OGL
{
	void GlClearErrors();
	bool GlLogCall(const char* function, const char* file, long line);

	class Renderer
	{
	public:
		Renderer();
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
		void Clear() const;
	};
}
