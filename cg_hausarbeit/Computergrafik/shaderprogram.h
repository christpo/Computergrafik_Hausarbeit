#pragma once
#include <string>
#include <exception>
#include <iostream>
#include "vector.h"
#include "color.h"
#include "Matrix.h"

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/GLUT.h>
#include <GL/GL.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	bool load(const char* VertexShader, const char* FragmentShader);
	bool loadVertexShader(const char* VertexShader);
	bool loadFragmentShader(const char* FragmentShader);
	bool compile(std::string* CompileErrors = NULL);

	GLint getParameterID(const char* ParameterName) const;

	void setParameter(GLint ID, float Param);
	void setParameter(GLint ID, int Param);
	void setParameter(GLint ID, const Vector& Param);
	void setParameter(GLint ID, const Color& Param);
	void setParameter(GLint ID, const Matrix& Param);

	void activate() const;
	void deactivate() const;

	GLint getFragmentShader()const;
	GLint getVertexShader()const;

	std::string readFile(const char *filePath);
	bool compileShader(const GLint& shader, std::string * CompileErrors);
	void deleteShader();

private:
	GLint m_VertexShader;
	GLint m_FragmentShader;
	GLint m_ShaderProgram;
};

