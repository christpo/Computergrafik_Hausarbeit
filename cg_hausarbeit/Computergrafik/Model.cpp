//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifdef WIN32
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GLUT.h>
#include <GL/GL.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

#include "Model.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>
#include <fstream>
#include <utility>
#include <algorithm>
#include <exception>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define SIZEFIT 100

using namespace std;

Vertex::Vertex()
{
}

Vertex::Vertex(const Vector& p, const Vector& n, float TexS, float TexT)
{
	Position = p;
	Normal = n;
	TexcoordS = TexS;
	TexcoordT = TexT;
}

BoundingBox::BoundingBox()
{
}
BoundingBox::BoundingBox(const Vector& min, const Vector& max) : Min(min), Max(max)
{
}

OrientedBoundingBox::OrientedBoundingBox()
{
}
OrientedBoundingBox::OrientedBoundingBox(const Vector& downLeft, const Vector& downRight, const Vector& upLeft, const Vector& upRight) : downLeft(downLeft), downRight(downRight), upLeft(upLeft), upRight(upRight)
{
}

Model::Model() : m_pMaterials(NULL), m_MaterialCount(0), m_VertexCount(0)
{

}

Model::~Model()
{
	glDeleteBuffers(1, &this->vertexBuffer);
	glDeleteBuffers(1, &this->indexBuffer);

	if (m_pMaterials)
		delete[] m_pMaterials;
}

bool Model::load(const char* Filename, bool FitSize, float scale, const char* VertexShader, const char* FragmentShader)
{
	//cout << Filename << endl;
	ifstream objFile;
	objFile.open(Filename, std::ifstream::in);

	if (!objFile.good()) {
		objFile.close();
		return false;
	}
	Vector normalold;
	string line;
	vector<string> token;
	vector<Vector> points;
	vector<Texcoord> textures;
	vector<Face> faces;
	vector<pair<int, string>> faceMtl;

	if (VertexShader == "" || FragmentShader == "") {
		this->useShader = false;
	}
	else {
		this->useShader = true;
	}

	if (useShader) {
		if (this->m_shaderprogram.load(VertexShader, FragmentShader)) {
			string errmsg;
			this->m_shaderprogram.compile(&errmsg);
			cout << errmsg << endl;
		}
		else {
			cout << "shader konnten nicht geladen werden" << endl;
		}
	}

	//startpunkt setzten
	this->m_faceMaterialMap.push_back(make_pair(faces.size(), ""));
	this->m_VertexCount = 0;

	while (!objFile.eof()) {

		getline(objFile, line);

		//wenn die zeile leer ist oder kommentar ->skip
		if (line[0] == 0 || line[0] == '#') {
			continue;
		}

		//cout << line << endl;
		stringstream ss(line);
		string cmd;
		ss >> cmd;


		if (cmd == "v") {
			getPoints(ss, points, FitSize, scale);
		}
		else if (cmd == "vt") {
			getTexCoords(ss, textures);
		}
		else if (cmd == "f") {

			vector<pair<unsigned int, unsigned int>>& idx = getFaceIdxs(line);

			for (unsigned int i = 1; i < idx.size() - 1; i++) {

				Face face = { idx[0].first ,idx[i].first ,idx[i + 1].first,	idx[0].second ,idx[i].second, idx[i + 1].second };
				faces.push_back(face);
			}
		}
		else if (cmd == "usemtl") {

			ss >> cmd;
			//wenn es ein material gibt dass zu anfang kommt->setzen
			if (this->m_faceMaterialMap.size() == 1 && faces.size() == 0) {

				this->m_faceMaterialMap[0].first = faces.size();
				this->m_faceMaterialMap[0].second = cmd;
			}
			else {

				this->m_faceMaterialMap.push_back(make_pair(faces.size() * 3, cmd));
			}
		}
		else if (cmd == "mtllib") {

			parseMtllib(ss, Filename);
		}
		else {

		}
	}

	objFile.close();

	//letzter
	this->m_faceMaterialMap.push_back(make_pair(faces.size() * 3, ""));

	//lensings vertices zuweisung und normalen berechnung
	m_VertexCount = faces.size() * 3;
	Vertex* m_pVertices = new Vertex[m_VertexCount];
	int* m_pIndices = new int[m_VertexCount];

	//bounding box
	Vector max = points[0];
	Vector min = points[0];

	for (unsigned int i = 0; i < faces.size(); i++) {

		unsigned int PosIdx0 = faces[i].pidx[0] - 1;
		unsigned int PosIdx1 = faces[i].pidx[1] - 1;
		unsigned int PosIdx2 = faces[i].pidx[2] - 1;

		unsigned int TexIdx0 = faces[i].tidx[0] - 1;
		unsigned int TexIdx1 = faces[i].tidx[1] - 1;
		unsigned int TexIdx2 = faces[i].tidx[2] - 1;

		m_pIndices[i * 3] = i * 3;
		m_pIndices[i * 3 + 1] = i * 3 + 1;
		m_pIndices[i * 3 + 2] = i * 3 + 2;

		Vector& a = m_pVertices[i * 3].Position = points[PosIdx0];
		Vector& b = m_pVertices[i * 3 + 1].Position = points[PosIdx1];
		Vector& c = m_pVertices[i * 3 + 2].Position = points[PosIdx2];

		m_pVertices[i * 3].TexcoordS = textures[TexIdx0].s;
		m_pVertices[i * 3 + 1].TexcoordS = textures[TexIdx1].s;
		m_pVertices[i * 3 + 2].TexcoordS = textures[TexIdx2].s;

		m_pVertices[i * 3].TexcoordT = textures[TexIdx0].t;
		m_pVertices[i * 3 + 1].TexcoordT = textures[TexIdx1].t;
		m_pVertices[i * 3 + 2].TexcoordT = textures[TexIdx2].t;

		//boundingbox
		checkMax(max, a);
		checkMax(max, b);
		checkMax(max, c);

		checkMin(min, a);
		checkMin(min, b);
		checkMin(min, c);

		this->m_Box.Max = max;
		this->m_Box.Min = min;

		Vector normal = (b - a).cross(c - a);

		try {
			normal.normalize();
		}
		catch (exception e) {
			normal = Vector(0, 0, 0);
		}

		m_pVertices[i * 3].Normal =
			m_pVertices[i * 3 + 1].Normal =
			m_pVertices[i * 3 + 2].Normal = normal;
	}

	// create gpu buffer for vertices
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_VertexCount, m_pVertices, GL_STATIC_DRAW);
	// create gpu buffer for indices
	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_VertexCount, m_pIndices, GL_STATIC_DRAW);

	delete[] m_pVertices;
	delete[] m_pIndices;

	return true;
}



const BoundingBox& Model::boundingBox() const
{
	return m_Box;
}


void Model::drawTriangles() const
{
	//wenn es kein material gibt

	//cout << "drawTriangles()" << endl;

	// inform the client that we want to use array buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// setup position & normal pointers
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(24));

	int texCount = 0;

	//bevor uniform variablen gesetzt werden können muss der shader aktiviert sein
	this->m_shaderprogram.activate();

	for (unsigned int k = 0; k < this->m_faceMaterialMap.size() - 1; k++) {

		//material setzen
		int start = this->m_faceMaterialMap[k].first;
		int end = this->m_faceMaterialMap[k + 1].first - 1;
		int ges = end - start;


		if (this->m_faceMaterialMap[k].second != "") {

			const Material& m = this->getMtl(this->m_faceMaterialMap[k].second);
			//textur
			if (m.isValidTexture()) {

				glActiveTexture(GL_TEXTURE0);
				glClientActiveTexture(GL_TEXTURE);

				m.appyTexture();

				GLint tex = this->m_shaderprogram.getParameterID("DiffuseTexture");
				glUniform1i(tex, 0);
			}
			//phong
			const Color* c = &m.getDiffuseColor();
			GLint dc = this->m_shaderprogram.getParameterID("DiffColor");
			glUniform3f(dc, c->R, c->G, c->B);

			c = &m.getSpecularColor();
			GLint sc = this->m_shaderprogram.getParameterID("SpecColor");
			glUniform3f(sc, c->R, c->G, c->B);

			GLint se = this->m_shaderprogram.getParameterID("SpecExp");
			glUniform1f(se, m.getSpecularExponent());

			c = &m.getAmbientColor();
			GLint ac = this->m_shaderprogram.getParameterID("AmbientColor");
			glUniform3f(ac, c->R, c->G, c->B);
		}
		//abschnitte zeichnen
		glDrawElements(GL_TRIANGLES, end - start +1, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * start));
	}

	this->m_shaderprogram.deactivate();

	//texturen deaktivieren
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // set modulate as default behaviour for unit 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::setBoundingBoxValues(const Vector & min, const Vector & max)
{
	this->m_Box.Max = max;
	this->m_Box.Min = min;
}

void Model::getPoints(stringstream &ss, vector<Vector> &points, bool fitsize, float scale) {

	float x, y, z;
	ss >> x >> y >> z;
	if (fitsize) {
		x /= scale;
		y /= scale;
		z /= scale;
	}
	points.push_back(Vector(x, y, z));
}

void Model::getTexCoords(stringstream &ss, vector<Texcoord> &texcoords) {

	Texcoord tex;
	ss >> tex.s >> tex.t;
	texcoords.push_back(tex);
}

vector<pair<unsigned int, unsigned int>> Model::getFaceIdxs(string line) {

	replace(line.begin(), line.end(), '/', ' ');
	stringstream idxfaces(line);
	string buf;
	unsigned int fidx, tidx;

	//f entfernen
	idxfaces >> buf;
	vector<pair<unsigned int, unsigned int>> indexes;

	while (idxfaces >> fidx) {
		idxfaces >> tidx;

		indexes.push_back(make_pair(fidx, tidx));
	}
	return indexes;
}

void Model::checkMin(Vector& a, Vector& b) {
	if (a.X > b.X) {
		a.X = b.X;
	}
	if (a.Y > b.Y) {
		a.Y = b.Y;
	}
	if (a.Z > b.Z) {
		a.Z = b.Z;
	}
}

void Model::checkMax(Vector& a, Vector& b) {

	if (a.X < b.X) {
		a.X = b.X;
	}
	if (a.Y < b.Y) {
		a.Y = b.Y;
	}
	if (a.Z < b.Z) {
		a.Z = b.Z;
	}
}

void Model::parseMtllib(stringstream& ss, const string& Filename) {

	//pfad bauen
	size_t found = Filename.find_last_of("/\\");
	string path = Filename.substr(0, found + 1);
	//cout << path << endl;

	string completepath = path;
	string mtllibfilename, cmd, line;
	ss >> mtllibfilename;

	completepath.append(mtllibfilename);
	//cout << completepath << endl;

	ifstream mtllibFile(completepath, std::ifstream::in);


	if (!mtllibFile.good()) {
		mtllibFile.close();
		cout << "material konnte nicht geladen werden" << endl;
		return;
	}

	/**/
	//material zählen
	int mtlcount = 0;
	while (!mtllibFile.eof()) {
		getline(mtllibFile, line);

		if (line != "") {
			stringstream ss(line);
			ss >> cmd;
			if (cmd == "newmtl") {
				mtlcount++;
			}
		}
	}
	mtllibFile.close();

	mtllibFile.open(completepath, std::ifstream::in);


	if (!mtllibFile.good()) {
		mtllibFile.close();
		cout << "material konnte nicht geladen werden" << endl;
		return;
	}

	this->m_pMaterials = new Material[mtlcount];

	/**/

	while (!mtllibFile.eof()) {
		getline(mtllibFile, line);

		if (line != "") {
			stringstream ss(line);
			ss >> cmd;
			if (cmd == "newmtl") {
				addMtls(mtllibFile, ss, path);
			}
		}
	}

	mtllibFile.close();
}

void Model::addMtls(ifstream& file, stringstream& ss, string filepath) {

	string line, cmd, name, map_kd;
	Color kd;
	Color ka;
	Color ks;
	float ns = 1;

	ss >> name;

	while (!file.eof()) {

		getline(file, line);

		if (line == "") {
			break;
		}

		stringstream sstr(line);
		sstr >> cmd;

		if (cmd == "Ni" || cmd == "d" || cmd == "Tr" || cmd == "Tf" || cmd == "illum" || cmd == "Tr" || cmd == "Ke") {
			continue;
		}

		if (cmd == "Kd") {
			sstr >> kd.R >> kd.G >> kd.B;
		}
		else if (cmd == "Ks") {
			sstr >> ks.R >> ks.G >> ks.B;
		}
		else if (cmd == "Ns") {
			sstr >> ns;
		}
		else if (cmd == "Ka") {
			sstr >> ka.R >> ka.G >> ka.B;
		}
		else if (cmd == "map_Kd") {
			sstr >> map_kd;
		}
	}
	this->m_pMaterials[this->m_MaterialCount].setDiffuseColor(kd);
	this->m_pMaterials[this->m_MaterialCount].setSpecularColor(ks);
	this->m_pMaterials[this->m_MaterialCount].setAmbientColor(ka);
	this->m_pMaterials[this->m_MaterialCount].setSpecularExponent(ns);
	this->m_pMaterials[this->m_MaterialCount].setName(name);

	if (map_kd != "") {
		filepath.append(map_kd);
		//mtl.setDiffuseTexture(filepath.c_str());
		this->m_pMaterials[this->m_MaterialCount].setDiffuseTexture(filepath.c_str());
	}
	this->m_MaterialCount++;

}


Material& Model::getMtl(const string& name) const {

	for (unsigned int i = 0; i < this->m_MaterialCount; i++) {

		if (this->m_pMaterials[i].getName() == name) {

			Material& m = this->m_pMaterials[i];
			return m;
		}
	}
}