#include "terrain.h"
#include "rgbimage.h"
#include <stdio.h>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define WDH 60


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}


bool Terrain::load(const char* HeightMap, const char* DetailMap1, const char* DetailMap2, const char* MixMap, float Width, float Depth, float heightMultiplier, const char* VertexShader, const char* FragmentShader) {

	if (!m_HeightMap.LoadFromBMP(HeightMap))
		return false;
	
	if (!m_DetailMap1.LoadFromBMP(DetailMap1))
		return false;

	if (!m_DetailMap2.LoadFromBMP(DetailMap2))
		return false;

	if (!m_MixMap.LoadFromBMP(MixMap))
		return false;

	if (this->m_shaderprogram.load(VertexShader, FragmentShader)) {
		string errmsg;
		this->m_shaderprogram.compile(&errmsg);
		cout << errmsg << endl;
	}
	else {
		cout << "shader konnten nicht geladen werden" << endl;
	}

	unsigned int imgheight = this->m_HeightMap.getRGBImage()->height();
	unsigned int imgwidth = this->m_HeightMap.getRGBImage()->width();

	const RGBImage* img = this->m_HeightMap.getRGBImage();

	int vertexcount = imgwidth*imgheight;
	TerrainVertex* vertices = new TerrainVertex [vertexcount];

	int detailimgheight = this->m_DetailMap1.getRGBImage()->height();
	int detailimgwidth = this->m_DetailMap1.getRGBImage()->width();

	int miximgheight = this->m_MixMap.getRGBImage()->height();
	int miximgwidth = this->m_MixMap.getRGBImage()->width();


	//vertexbuffer füllen
	int verticesIndex = 0;
	for (unsigned int i = 0; i < imgheight; i++) {
		for (unsigned int k = 0; k < imgwidth; k++) {

			//zentrieren und skalieren
			float x = (((float)k - (imgwidth/2))/ imgwidth)*Width;
			float z = (((float)i - (imgheight / 2))/ imgheight)*Depth;

			//höhe holen
			float height = img->getPixelColor(k, i).R;

			//position
			Vector pos(x, height * heightMultiplier, z);

			//textur
			float u0 = ((float)k / miximgwidth);
			float v0 = ((float)i / miximgheight);

			float u1 = WDH * u0;
			float v1 = WDH * v0;

			vertices[verticesIndex].Pos = pos;
			vertices[verticesIndex].u0 = u0;
			vertices[verticesIndex].u1 = u1;
			vertices[verticesIndex].v0 = v0;
			vertices[verticesIndex++].v1 = v1;
		
		}
	}
	

	this->indexcount = (imgheight-1)*(imgwidth-1) * 6;
	int* indices = new int[indexcount];
	int indicesIndex = 0;

	//indexbuffer füllen
	for (unsigned int i = 0; i < imgheight - 1; i++) {
		for (unsigned int k = 0; k < imgwidth - 1; k++) {
			
			int start = i * imgwidth + k;
			int verta = start + imgwidth;
			int vertb = start + 1;
			int vertc = start;
			int vertd = start + 1 + imgwidth;

			indices[indicesIndex++] = start + imgwidth;
			indices[indicesIndex++] = start + 1;
			indices[indicesIndex++] = start;
			

			indices[indicesIndex++] = start + imgwidth;
			indices[indicesIndex++] = start + 1 + imgwidth;
			indices[indicesIndex++] = start + 1;
			
		}
	}

	//shame
	for (int i = 0; i < imgheight; i++) {
		for (int k = 0; k < imgwidth; k++) {
			//wenn untenlinks
			if (i == 0 && k == 0) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& above = vertices[imgwidth].Pos;
				Vector& right = vertices[k + 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalCornerLURO(center, above, right);
			}
			//wenn obenrechts
			else if (i == imgheight - 1 && k == imgwidth - 1) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& under = vertices[i * imgwidth + k -1].Pos;
				Vector& left = vertices[(i - 1) * imgwidth + k].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalCornerLURO(center, under, left);
			}
			//wenn obenlinks
			else if (i == imgheight - 1 && k == 0) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& under = vertices[(i - 1) * imgwidth].Pos;
				Vector& rightunder = vertices[(i - 1) * imgwidth + 1].Pos;
				Vector& right = vertices[i * imgwidth + 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalCornerLORU(center, under, rightunder, right);
			}
			//wenn unten rechts
			else if (i == 0 && k == imgwidth - 1) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& above= vertices[imgwidth + k].Pos;
				Vector& leftabove = vertices[imgwidth + k - 1].Pos;
				Vector& left = vertices[k - 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalCornerLORU(center, above, leftabove, left);
			}
			//wenn linker rand
			else if (k == 0) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& above = vertices[(i + 1) * imgwidth + k].Pos;
				Vector& rightunder = vertices[(i - 1) * imgwidth + k + 1].Pos;
				Vector& under = vertices[(i - 1) * imgwidth + k].Pos;
				Vector& right = vertices[i * imgwidth + k + 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalBorder(center, under, rightunder, right, above);
			}
			//wenn unterer rand
			else if (i == 0) {

				Vector& center = vertices[k].Pos;
				Vector& above = vertices[imgwidth + k].Pos;
				Vector& leftabove = vertices[imgwidth + k - 1].Pos;
				Vector& left = vertices[k - 1].Pos;
				Vector& right = vertices[k + 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalBorder(center, left, leftabove, above, right);
			}
			//wenn rechter rand
			else if (k == imgwidth - 1) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& above = vertices[(i + 1) * imgwidth + k].Pos;
				Vector& leftabove = vertices[(i + 1) * imgwidth + k - 1].Pos;
				Vector& under = vertices[(i - 1) * imgwidth + k].Pos;
				Vector& left = vertices[i * imgwidth + k - 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalBorder(center, above, leftabove, left, under);
			}
			//wenn oberer rand
			else if (i == imgheight - 1) {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& under = vertices[(i-1) * imgwidth + k].Pos;
				Vector& rightunder = vertices[(i - 1) * imgwidth + k + 1].Pos;
				Vector& left = vertices[i * imgwidth + k - 1].Pos;
				Vector& right = vertices[i * imgwidth + k + 1].Pos;

				vertices[i * imgwidth + k].Normal = calcNormalBorder(center, right, rightunder, under, left);
			}
			//wenn iwo in der mitte
			else {

				Vector& center = vertices[i * imgwidth + k].Pos;
				Vector& under = vertices[(i - 1) * imgwidth + k].Pos;
				Vector& left = vertices[i * imgwidth + k - 1].Pos;
				Vector& leftabove = vertices[(i + 1) * imgwidth + k - 1].Pos;
				Vector& above = vertices[(i + 1) * imgwidth + k].Pos;
				Vector& right = vertices[i * imgwidth + k + 1].Pos;
				Vector& rightunder = vertices[(i - 1) * imgwidth + k + 1].Pos;
				

				vertices[i * imgwidth + k].Normal = calcNormal(center, under, left, leftabove, above, right, rightunder);
			}

			//cout << "i = " << i << " k = " << k << endl;
		}
	}
	
	
	// create gpu buffer for vertices
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertexcount, vertices, GL_STATIC_DRAW);
	// create gpu buffer for indices
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexcount, indices, GL_STATIC_DRAW);

	delete[] vertices;
	delete[] indices;

	return true;
}


void Terrain::draw() {

	// inform the client that we want to use array buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// setup position & normal pointers
	glVertexPointer(3, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(12));

	this->m_shaderprogram.activate();
	
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(32));
	this->m_DetailMap1.apply();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLint t1 = this->m_shaderprogram.getParameterID("Tex1");
	glUniform1i(t1, 0);

	glActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(32));
	this->m_DetailMap2.apply();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLint t2 = this->m_shaderprogram.getParameterID("Tex2");
	glUniform1i(t2, 1);

	glActiveTexture(GL_TEXTURE2);
	glClientActiveTexture(GL_TEXTURE2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(24));
	this->m_MixMap.apply();
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	GLint t3 = this->m_shaderprogram.getParameterID("Mix");
	glUniform1i(t3, 2);
	/*
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE1);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE0);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE2);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR);
	
	glActiveTexture(GL_TEXTURE3);
	glClientActiveTexture(GL_TEXTURE3);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), BUFFER_OFFSET(24)); 
	this->m_DetailMap1.apply();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	*/
	// we draw our plane
	glDrawElements(GL_TRIANGLES, this->indexcount, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	
	// disable states in reverse order
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE2);
	glClientActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glClientActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // set modulate as default behaviour for unit 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->m_shaderprogram.deactivate();
}

Vector Terrain::calcNormalCornerLURO(const Vector& center, const Vector& dir1, const Vector& dir2) {

	Vector normal = (dir1 - center).cross(dir2 - center);
	normal.normalize();
	return normal;
}

Vector Terrain::calcNormalCornerLORU(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3) {

	Vector normal1 = (dir1 - center).cross(dir2 - center);
	Vector normal2 = (dir2 - center).cross(dir3 - center);
	Vector resnormal = (normal1 + normal2) / 2;
	resnormal.normalize();
	return resnormal;
}

Vector Terrain::calcNormalBorder(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3, const Vector& dir4) {

	Vector normal1 = (dir1 - center).cross(dir2 - center);
	Vector normal2 = (dir2 - center).cross(dir3 - center);
	Vector normal3 = (dir3 - center).cross(dir4 - center);
	Vector resnormal = (normal1 + normal2 + normal3) / 3;
	resnormal.normalize();
	return resnormal;
}

Vector Terrain::calcNormal(const Vector& center, const Vector& dir1, const Vector& dir2, const Vector& dir3, const Vector& dir4, const Vector& dir5, const Vector& dir6) {

	Vector normal1 = (dir1 - center).cross(dir2 - center);
	Vector normal2 = (dir2 - center).cross(dir3 - center);
	Vector normal3 = (dir3 - center).cross(dir4 - center);
	Vector normal4 = (dir4 - center).cross(dir5 - center);
	Vector normal5 = (dir5 - center).cross(dir6 - center);
	Vector resnormal = (normal1 + normal2 + normal3 +normal4 + normal5) / 5;
	resnormal.normalize();
	return resnormal;
}