#include <iostream>

#include "Renderer.h"

namespace OGL
{
	void GlClearErrors()
	{
		while (glGetError());
	}

	bool GlLogCall(const char* function, const char* file, long line)
	{
		bool isOk = 1;
		while (GLenum error = glGetError())
		{
			isOk = 0;
			std::cout << "[OpenGL error] (" << error << ")\n";
		}
		if (!isOk)
			std::cout << "file:\t" << file << "\nfunc:\t" << function << "\nline:\t" << line << "\n\n";
		return isOk;
	}

	Renderer::Renderer()
	{

	}

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();
		GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() const
	{
		GlCall(glClear(GL_COLOR_BUFFER_BIT));
	}
}
