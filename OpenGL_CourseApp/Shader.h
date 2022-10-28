#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include <GL/glew.h>

class Shader
{

public:
	Shader();

	~Shader();
	
	void CreateFromFile(const char* vShaderFilePath, const char* fShaderFilePath);

	string ReadFile(const char* filePath);

	GLuint GetModelLocation() const { return uniformModel; }

	GLuint GetProjectionLocation() const { return uniformProjection; }

	void UseShader();

	void ClearShader();

private:

	GLuint shaderId, uniformModel, uniformProjection;

	void CompileShader(const char* vShader, const char* fShader);

	void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);

};