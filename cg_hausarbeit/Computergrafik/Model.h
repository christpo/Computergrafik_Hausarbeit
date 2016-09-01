//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include "vector.h"
#include "color.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "material.h"
#include <map>
#include "shaderprogram.h"

using namespace std;

struct Face
{
	unsigned int pidx[3], tidx[3];
};

struct Texcoord
{
	float s, t;
};

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    Vector Min;
    Vector Max;
};

class OrientedBoundingBox
{
public:
	OrientedBoundingBox();
	OrientedBoundingBox(const Vector& downLeft, const Vector& downRight, const Vector& upLeft, const Vector& upRight);
	Vector downLeft;
	Vector downRight;
	Vector upLeft;
	Vector upRight;
};

class Model
{
public:
    Model();
    ~Model();
    const BoundingBox& boundingBox() const;
    bool load(const char* Filename, bool FitSize, float scale, const char* VertexShader, const char* FragmentShader);
    void drawLines() const;
    void drawTriangles() const;
	void setBoundingBoxValues(const Vector& min, const Vector& max);
	OrientedBoundingBox obb;
	
protected:
	//selfmade
	void getPoints(stringstream &ss, vector<Vector> &points, bool fitsize, float scale);
	void getTexCoords(stringstream &ss, vector<Texcoord> &texcoords);
	vector<pair<unsigned int, unsigned int>> getFaceIdxs(string line);
	void checkMin(Vector& a, Vector& b);
	void checkMax(Vector& a, Vector& b);
	void parseMtllib(stringstream& ss, const string& Filename);
	void addMtls(ifstream& file, stringstream& ss, string filepath);
	void draw(int start, int end) const;
	void draw() const;
	Material& getMtl(const string& name) const;


    Material* m_pMaterials;
    unsigned int m_MaterialCount;
    unsigned int m_VertexCount;
    BoundingBox m_Box;

	//selfmade
	vector<pair<int, string>> m_faceMaterialMap;

	ShaderProgram m_shaderprogram;
	bool useShader;

	GLuint vertexBuffer, indexBuffer;
};

#endif /* defined(__RealtimeRending__Model__) */
