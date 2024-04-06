#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace OGL
{
	class Shader
	{
		unsigned int m_RendererID;
		std::string m_VertexPath, m_FragmentPath;
		std::unordered_map<std::string, int> m_UniformLocationCache;

	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind() const;
		static void Unbind();

		void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
		void SetUniform1i(const std::string& name, int v1);
		void SetUniform1iv(const std::string& name, int count, int* v1);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

		std::pair<std::string, std::string> ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	protected:
		unsigned int GetLocation(const std::string& name);
	};
}
