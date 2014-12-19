#include <fstream>
#include <vector>

#include "Errors.h"
#include "ShaderProgram.h"


namespace GEngine {
	ShaderProgram::ShaderProgram() :
		m_object(0) {
		// Create the program object
		m_object = glCreateProgram();
		if (m_object == 0)
			throw std::runtime_error("glCreateProgram failed");
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_object);
	}

	void ShaderProgram::linkShaders(const std::vector<Shader>& shaders) {
		// Attach all shaders
		for (unsigned i = 0; i < shaders.size(); ++i) {
			glAttachShader(m_object, shaders[i].m_object);
		}

		// Link the program
		glLinkProgram(m_object);

		// Detach all shaders
		for (unsigned i = 0; i < shaders.size(); ++i) {
			glDetachShader(m_object, shaders[i].m_object);
		}

		// Error checking
		GLint isLinked = 0;
		glGetProgramiv(m_object, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_object, maxLength, &maxLength, &errorLog[0]);

			for (unsigned i = 0; i < shaders.size(); ++i) {
				glDeleteShader(shaders[i].m_object);
			}

			glDeleteProgram(m_object);

			// Print the error log and quit.
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
		}
	}

	// Adds an attribute to the shader. Should be called between compiling and linking
	void ShaderProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_object, m_numAttributes++, attributeName.c_str());
	}

	GLint ShaderProgram::getUniformLocation(const std::string& uniformName) {
		GLint uniform = glGetUniformLocation(m_object, uniformName.c_str());
		if (uniform == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return uniform;
	}

	// Enable the shader and all its attributes
	void ShaderProgram::enable() {
		glUseProgram(m_object);
		// Enable all the attributes we added with addAttribute
		for (int i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	// Disable the shader
	void ShaderProgram::disable() {
		glUseProgram(0);
		for (int i = 0; i < m_numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}
}