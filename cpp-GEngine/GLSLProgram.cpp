#include <fstream>
#include <vector>

#include "Errors.h"
#include "GLSLProgram.h"


namespace GEngine {
GLSLProgram::GLSLProgram() :
	_numAttributes(0),
	_programID(0),
	_vertexShaderID(0),
	_fragmentShaderID(0) {
}

GLSLProgram::~GLSLProgram() {
}

// Compiles the shaders into a form that the GPU can understand
void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

	// Get a program object
	_programID = glCreateProgram();

	// Create the vertex shader object, and store its ID
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fatalError("Vertex shader failed to be created!");
	}

	// Create the fragment shader object, and store its ID
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fatalError("Fragment shader failed to be created!");
	}

	// Compile each shader
	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::linkShaders() {
	// Attach the shaders to the program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	// Link the program
	glLinkProgram(_programID);

	// Error checking
	// Note the different functions here: glGetProgram* instead of glGetShader*
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		// Print the error log and quit.
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shaders failed to link!");
	}

	// Always detach shaders after a successful link
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

// Adds an attribute to the shader. Should be called between compiling and linking
void GLSLProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}

// Enable the shader and all its attributes
void GLSLProgram::use() {
	glUseProgram(_programID);
	// Enable all the attributes we added with addAttribute
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

// Disable the shader
void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

// Compiles a single shader file
void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {
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
	glShaderSource(id, 1, &contentsPtr, nullptr);

	// Compile the shader
	glCompileShader(id);

	// Check for errors
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Exit with failure
		glDeleteShader(id);

		// Print error log and quit
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + filePath + " failed to compile!");
	}
}
}