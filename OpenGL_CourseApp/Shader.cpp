#include "Shader.h"

Shader::Shader()
{
	shaderId = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromFile(const char* vShaderFilePath, const char* fShaderFilePath)
{
	string vShader = ReadFile(vShaderFilePath);
	string fShader = ReadFile(fShaderFilePath);

	CompileShader(vShader.c_str(), fShader.c_str());
}

string Shader::ReadFile(const char* filePath)
{
	string content;
	ifstream fileStream(filePath, ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s. File does not exist.", filePath);
		return "";
	}

	string line;
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::UseShader()
{
	glUseProgram(shaderId);
}

void Shader::ClearShader()
{
	if (shaderId != 0)
	{
		glDeleteProgram(shaderId);
		shaderId = 0;
	}

	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
}

void Shader::CompileShader(const char* vShader, const char* fShader)
{
	shaderId = glCreateProgram();

	if (!shaderId)
	{
		printf("Error creating shader program.\n");
		return;
	}

	AddShader(shaderId, vShader, GL_VERTEX_SHADER);
	AddShader(shaderId, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s\n", eLog);
		return;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformView = glGetUniformLocation(shaderId, "view");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
}

void Shader::AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s\n", shaderType, eLog);
		return;
	}

	glAttachShader(shaderProgram, shader);
}