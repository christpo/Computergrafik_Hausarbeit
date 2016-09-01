#include "rgbimage.h"
#include <stdexcept>
#include <stdio.h>
#include <Windows.h>


RGBImage::RGBImage(unsigned int Width, unsigned int Height) {

	this->m_Height = Height;
	this->m_Width = Width;
	this->m_Image = new Color[Width*Height];
}

RGBImage::~RGBImage() {
	
	delete[] this->m_Image;
	
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {

	if (x < 0 || x > this->m_Width - 1 || y < 0 || y > this->m_Height - 1) {

		throw std::out_of_range("array out of bound exception");
	}
	else {

		this->m_Image[y * this->m_Width + x] = c;
	}
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {

	if (x < 0 || x > this->m_Width - 1 || y < 0 || y > this->m_Height - 1) {

		throw std::out_of_range("array out of bound exception");
	}
	else {

		return this->m_Image[y * this->m_Width + x];
	}
}

bool RGBImage::saveToDisk(const char* Filename) {

	FILE *file;
	errno_t errcode = fopen_s(&file, Filename, "wb");


	bool success = false;

	if (errcode != 0) {

		perror("Error opening file");
	}
	else {

		BITMAPFILEHEADER fileheader = { 0x4D42, 54 + this->m_Width * this->m_Height * 4, 0, 0, 54 };
		BITMAPINFOHEADER infoheader = { 40, this->m_Width, this->m_Height * (-1), 1, 24, BI_RGB, 0, 0, 0, 0, 0 };

		fwrite(&fileheader, sizeof(char), sizeof(fileheader), file);
		fwrite(&infoheader, sizeof(char), sizeof(infoheader), file);

		for (unsigned int i = 0; i < this->m_Height; i++) {

			for (unsigned int k = 0; k < this->m_Width; k++) {

				char color[3] = { this->convertColorChannel(this->m_Image[i*this->m_Width + k].B),
					this->convertColorChannel(this->m_Image[i*this->m_Width + k].G),
					this->convertColorChannel(this->m_Image[i*this->m_Width + k].R) };

				fwrite(color, sizeof(char), sizeof(color), file);
			}
		}

		fclose(file);

		success = true;
	}

	return success;
}

unsigned int RGBImage::width() const {

	return this->m_Width;
}

unsigned int RGBImage::height() const {

	return this->m_Height;
}

unsigned char RGBImage::convertColorChannel(float f) {


	if (f < 0.0f) {

		f = 0.0f;
	}
	else if (f > 1.0f) {

		f = 1.0f;
	}

	return f * 255;
}

RGBImage::RGBImage(const RGBImage& other) {
	*this = other;
}

RGBImage& RGBImage::operator=(const RGBImage & other) {
	if (this == &other){
		return *this;
	}
	delete this->m_Image;
	this->m_Height = other.m_Height;
	this->m_Width = other.m_Width;
	int size = this->m_Width*this->m_Height;
	this->m_Image = new Color[size];
	for (int i = 0; i < size; i++) {
		this->m_Image[i] = other.m_Image[i];
	}
}



