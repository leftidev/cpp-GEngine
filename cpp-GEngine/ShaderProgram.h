#pragma once

#include <string>

#include <GL/glew.h>

#include "Shader.h"


namespace GEngine {
	// This class handles the compilation, linking, and usage of a GLSL shader program.
	// Reference: http://www.opengl.org/wiki/Shader_Compilation
	class ShaderProgram {
	public:
		ShaderProgram();
		~ShaderProgram();

		void linkShaders(const std::vector<Shader>& shaders);
		void addAttribute(const std::string& attributeName);
		GLint getUniformLocation(const std::string& uniformName);
		void enable();
		void disable();

		GLuint m_object;
		int m_numAttributes;
	private:

	};
}