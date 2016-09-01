#pragma once
#ifndef __RealtimeRending__TerrainModel__
#define __RealtimeRending__TerrainModel__

#include <iostream>
#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/GLUT.h>
#include <GL/GL.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

#include "vector.h"
#include "Texture.h"
#include "shaderprogram.h"


struct TerrainVertex
{
	Vector Pos;
	Vector Normal;
	float u0, v0;
	float u1, v1;
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	bool load(const char* HeightMap, const char* DetailMap1, const char* DetailMap2, const char* MixMap, float Width, float Depth, float heightMultiplier, const char* VertexShader, const char* FragmentShader);
	void draw();

	Vector calcNormalCornerLURO(const Vector& center, const Vector& dir1, const Vector& dir2);
	Vector calcNormalCornerLORU(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3);
	Vector calcNormalBorder(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3, const Vector& dir4);
	Vector calcNormal(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3, const Vector& dir4, const Vector& dir5, const Vector& dir6);

protected:
	Texture m_HeightMap;
	Texture m_DetailMap1;
	Texture m_DetailMap2;
	Texture m_MixMap;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
	unsigned int indexcount;

	ShaderProgram m_shaderprogram;
};

#endif /* defined(__RealtimeRending__TerrainModel__) */