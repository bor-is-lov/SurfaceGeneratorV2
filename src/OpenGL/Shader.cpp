#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_RendererID(0)
{
	auto [vertexSource, fragmentSource] = ParseShader(vertexPath, fragmentPath);
	m_RendererID = CreateShader(vertexSource, fragmentSource);
	GlCall(glUseProgram(m_RendererID));
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind()
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
	GlCall(glUniform4f(GetLocation(name), v1, v2, v3, v4));
}

void Shader::SetUniform1i(const std::string& name, int v1)
{
	GlCall(glUniform1i(GetLocation(name), v1));
}

void Shader::SetUniform1iv(const std::string& name, int count, int* v1)
{
	GlCall(glUniform1iv(GetLocation(name), count, v1));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GlCall(glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

std::pair<std::string, std::string> Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string line;
	std::stringstream shaders[2];

	std::ifstream fin(vertexPath);
	while (getline(fin, line))
		shaders[0] << line << '\n';
	fin.close();

	fin.open(fragmentPath);
	while (getline(fin, line))
		shaders[1] << line << '\n';
	fin.close();

	return { shaders[0].str(), shaders[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	const char* src = source.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* errMsg = new char[length];
		glGetShaderInfoLog(id, length, &length, errMsg);
		std::cout << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n" << errMsg << '\n';
		delete[] errMsg;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::GetLocation(const std::string& name)
{
	unsigned int location;
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		location = m_UniformLocationCache[name];
	else
	{
		location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;
	}
	return location;
}
