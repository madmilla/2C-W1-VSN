//header guard at start of header file
#ifndef image_H
#define image_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "FreeImage.h"
#include <string>
#include "util.h"
#include "histogram.h"


/**
* image.h
*
*
* Author: Lars Veenendaal
* TIV-2A 1633223
*
* Making images since march 2014
*
*/
struct Pixel {   
	unsigned char RED;   
	unsigned char BLUE;
	unsigned char GREEN;
	int array;
};
 class Image{
 private:
	int width, height;
	Pixel ** pixelData;
public:
	Image(){};
	Image(FIBITMAP *);
	Image(Image &img);
	Image(const Image *img);
	~Image();
	Image& operator= (const Image&);
	FIBITMAP * UpdateForSaving(FIBITMAP *);
	Pixel** getPixelData();
	void ConvertToGreyscale(int);
	void ConvertToGreyscale_lightness();
	void ConvertToGreyscale_luminosity();
	void ConvertToGreyscaleWithDensityArray(int(&result_array)[256], int(&result_array_10)[11], int);
	void equalizeHistogram(int eq_array[], int);
	void set_values(int, int);
	int getHeight()const;
	int getWidth()const;
	Pixel getPixel(int, int)const;
	void setPixel(int, int, Pixel);
	void getRGBChannelSeperatedIntensity(char * filename)const;
	void getRedChannelOnly()const;
	void getGreenChannelOnly()const;
	void getBlueChannelOnly()const;
	void save_image(FIBITMAP * temp, char *prefix, char *filename);
	unsigned char getRed(int, int)const;
	unsigned char getGreen(int, int)const;
	unsigned char getBlue(int, int)const;
	unsigned char getGreyScalePixel(int row, int col);
	void setRed(int, int, int);
	void setGreen(int, int, int);
	void setBlue(int, int, int);
	void setGreyScalePixel(int row, int col, int val);

};

#endif