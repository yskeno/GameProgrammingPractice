#pragma once
#include <string>
#include <GL/glew.h>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();

	// Load the vertex/fragment shaders with the given names
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	// Set this as the active shader program
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);

private:
	// Tries to compile the specified shader
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	// Tests whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// Tests whether vertex/fragment programs link
	bool IsValidProgram();

	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

