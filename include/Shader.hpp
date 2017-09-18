#pragma once

#include <GL/glew.h>
#include <util.h>
#include "../GraphicsEngine/glm/glm.hpp"
#include "../GraphicsEngine/glm/gtc/type_ptr.hpp"

class Shader
{
	private:
		GLuint      _programId = 0;
		bool        _isEnabled = false;
	private:
		bool        _createShaders(GLuint& vert, GLuint& frag, char *vertData, char *fragData);
		bool        _compileShader(GLuint& shader, const char *name, const char *data);
		bool        _createProgram(GLuint& vert, GLuint& frag, char *vertData, char *fragData);
	public:
		Shader() = default;
		Shader(const char *vertPath, const char *fragPath);
		~Shader();

		bool    initShader(const char *vertPath, const char *fragPath);
		void    enable();
		void    disable();
		GLint   getAttribLocation(char *name);
		GLint   getUniformLocation(GLchar *name);
		void    setUniform1i(GLchar *name, int value);
		void    setUniform1f(GLchar *name, float value);
		void    setUniform3f(GLchar *name, glm::vec3& vec);
		void    setUniform4f(GLchar *name, glm::vec4& vec);
		void    setUniformMat4(GLchar *name, glm::mat4 matrix);
};
