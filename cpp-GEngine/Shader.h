#pragma once

#include <GL/glew.h>

#include <string>


namespace GEngine {
	class Shader {
	public:
		Shader(const std::string& shaderCode, GLenum shaderType);
		~Shader();

		GLuint m_object;
	};
}