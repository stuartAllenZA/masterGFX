#include <Shader.hpp>

Shader::Shader(const char *vertPath, const char *fragPath)
{
	initShader(vertPath, fragPath);
}

Shader::~Shader()
{
	if (_programId != 0)
		glDeleteProgram(_programId);
}

bool Shader::initShader(const char *vertPath, const char *fragPath)
{
	char    *vertData;
	char    *fragData;
	GLuint  vertShader;
	GLuint  fragShader;

	if ((vertData = readFile(vertPath)) == nullptr)
		return false;
	if ((fragData = readFile(fragPath)) == nullptr)
	{
		free(vertData);
		return false;
	}
	if (!_createShaders(vertShader, fragShader, vertData, fragData))
		return false;
	if (!_compileShader(vertShader, vertPath, (const char *)vertData))
	{
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		free(vertData);
		free(fragData);
		return false;
	}
	if (!_compileShader(fragShader, fragPath, (const char *)fragData))
	{
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		free(vertData);
		free(fragData);
		return false;
	}
	return _createProgram(vertShader, fragShader, vertData, fragData);
}

bool Shader::_createProgram(GLuint &vert, GLuint &frag, char *vertData, char *fragData)
{
	_programId = glCreateProgram();
	glAttachShader(_programId, vert);
	glAttachShader(_programId, frag);
	glLinkProgram(_programId);
	glValidateProgram(_programId);
	glDeleteShader(vert);
	glDeleteShader(frag);
	free(vertData);
	free(fragData);
	return true;
}

bool Shader::_createShaders(GLuint &vert, GLuint &frag, char *vertData, char *fragData)
{
	if ((vert = glCreateShader(GL_VERTEX_SHADER)) == 0)
	{
		free(vertData);
		free(fragData);
		std::cerr << "Failed to create vertex shaders!" << std::endl;
		return false;
	}
	if ((frag = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
	{
		free(vertData);
		free(fragData);
		glDeleteShader(vert);
		std::cerr << "Failed to create fragment shaders!" << std::endl;
		return false;
	}
	return true;
}

bool Shader::_compileShader(GLuint &shader, const char *name, const char *data)
{
	glShaderSource(shader, 1, &data, nullptr);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::string     error = std::string();
		error.resize((unsigned long)length);
		glGetShaderInfoLog(shader, length, &length, &error[0]);
		std::cerr << "Failed to compile " << name << " shader!" << std::endl <<
			error << std::endl;
		glDeleteShader(shader);
		return false;
	}
	return true;
}

void Shader::enable()
{
	if (!_isEnabled)
		glUseProgram(_programId);
}

void Shader::disable()
{
	glUseProgram(0);
}

GLint Shader::getAttribLocation(char *name)
{
	return glGetAttribLocation(_programId, name);
}

GLint Shader::getUniformLocation(GLchar *name)
{
	return glGetUniformLocation(_programId, name);
}

void Shader::setUniform1i(GLchar *name, int value)
{
	enable();
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform3f(GLchar *name, glm::vec3 &vec)
{
	enable();
	glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(GLchar *name, glm::vec4 &vec)
{
	enable();
	glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniformMat4(GLchar *name, glm::mat4 matrix)
{
	enable();
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform1f(GLchar *name, float value)
{
	enable();
	//std::cout << "uniform " << getUniformLocation(name) << std::endl;
	glUniform1f(getUniformLocation(name), value);
}
