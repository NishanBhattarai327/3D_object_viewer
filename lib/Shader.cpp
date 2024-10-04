#include "self/Shader.h"

#include "self/error.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// retrive shaders from source code
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open file
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		
		// read file buffer content into stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handler
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		std::cout << "Vertex Shader path: " << vertexPath << std::endl;
		std::cout << "Fragment Shader path: " << fragmentPath << std::endl;
		return;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vs;
	GLCall(vs = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vs, 1, &vShaderCode, nullptr));
	GLCall(glCompileShader(vs));
	checkShaderCompilationError(vs, "VERTEX");

	unsigned int fs;
	GLCall(fs = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fs, 1, &fShaderCode, nullptr));
	GLCall(glCompileShader(fs));
	checkShaderCompilationError(fs, "FRAGMENT");

	GLCall(Shader::id = glCreateProgram());
	GLCall(glAttachShader(Shader::id, vs));
	GLCall(glAttachShader(Shader::id, fs));
	GLCall(glLinkProgram(Shader::id));
	checkProgramLinkingError();
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

// use/activate the shader
void Shader::use() const {
	GLCall(glUseProgram(Shader::id));
}

// utility uniform functions
void Shader::setBool(const std::string& name, bool value) const {
	GLCall(glUniform1i(glGetUniformLocation(Shader::id, name.c_str()), (int)value));
}
void Shader::setInt(const std::string& name, int value) const {
	GLCall(glUniform1i(glGetUniformLocation(Shader::id, name.c_str()), value));
}
void Shader::setFloat(const std::string& name, float value) const {
	GLCall(glUniform1f(glGetUniformLocation(Shader::id, name.c_str()), value));
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	GLCall(glUniform2fv(glGetUniformLocation(Shader::id, name.c_str()), 1, &value[0]));
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	GLCall(glUniform2f(glGetUniformLocation(Shader::id, name.c_str()), x, y));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	GLCall(glUniform3fv(glGetUniformLocation(Shader::id, name.c_str()), 1, &value[0]));
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	GLCall(glUniform3f(glGetUniformLocation(Shader::id, name.c_str()), x, y, z));
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GLCall(glUniform4fv(glGetUniformLocation(Shader::id, name.c_str()), 1, &value[0]));
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	GLCall(glUniform4f(glGetUniformLocation(Shader::id, name.c_str()), x, y, z, w));
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	GLCall(glUniformMatrix2fv(glGetUniformLocation(Shader::id, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	GLCall(glUniformMatrix3fv(glGetUniformLocation(Shader::id, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(Shader::id, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}



void Shader::checkShaderCompilationError(unsigned int shader, const char* shaderName) const
{
	int success;
	char infoLog[512];

	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success) {
		GLCall(glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog));
		std::cout << "ERROR::" << shaderName << "-SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "SUCCESS::" << shaderName << "-SHADER::COMPILATION" << std::endl;
	}
}

void Shader::checkProgramLinkingError() const
{
	int success;
	char infoLog[512];

	GLCall(glGetProgramiv(Shader::id, GL_LINK_STATUS, &success));
	if (!success) {
		GLCall(glGetProgramInfoLog(Shader::id, sizeof(infoLog), NULL, infoLog));
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else {
		std::cout << "SUCCESS::PROGRAM::LINKING" << std::endl;
	}
}
