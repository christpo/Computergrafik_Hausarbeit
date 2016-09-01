#pragma once

#include "color.h"

class RGBImage
{
	public:
		RGBImage(unsigned int Width, unsigned int Height);
		~RGBImage();

		RGBImage(const RGBImage& other);
		RGBImage& operator=(const RGBImage & other);

		void setPixelColor(unsigned int x, unsigned int y, const Color& c);
		const Color& getPixelColor(unsigned int x, unsigned int y) const;
		bool saveToDisk(const char* Filename);
		unsigned int width() const;
		unsigned int height() const;

		static unsigned char convertColorChannel(float f);

	protected:
		Color* m_Image;
		unsigned int m_Height;
		unsigned int m_Width;
};

