#include <fstream>
#include <vector>

#include "Errors.h"
#include "Shader.h"


namespace GEngine {
	Shader::Shader(const std::string& filePath, GLenum shaderType) :
		m_object(0) {
		// Create the vertex shader object, and store its ID
		m_object = glCreateShader(shaderType);
		if (m_object == 0) {
			throw std::runtime_error("glCreateShader failed");
		}
		// TODO: Refactor the opening of file to a "ResourcePath"-function..
		// Open the file
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}
		// File contents stores all the text in the file
		std::string fileContents = "";
		// Line is used to grab each line of the file
		std::string line;

		// Get all the lines in the file and add it to the contents
		while (std::getline(vertexFile, line)) {
			fileContents += line + "\n";
		}

		vertexFile.close();

		// Get a pointer to our file contents c string;
		const char* contentsPtr = fileContents.c_str();
		// Tell OpenGL that we want to use fileContents as the contents of the shader file
		glShaderSource(m_object, 1, &contentsPtr, nullptr);

		// Compile the shader
		glCompileShader(m_object);

		// Check for errors
		GLint success = 0;
		glGetShaderiv(m_object, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(m_object, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(m_object, maxLength, &maxLength, &errorLog[0]);

			// Exit with failure
			glDeleteShader(m_object);

			// Print error log and quit
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile!");
		}
	}

	Shader::~Shader() { }
}