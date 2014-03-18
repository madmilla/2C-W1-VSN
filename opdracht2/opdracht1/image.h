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
class Image {
	int width, height;
	Pixel ** pixelData;
public:
	Image(FIBITMAP *);
	Image(Image &img);
	~Image();
	Image& operator= (const Image&);
	FIBITMAP * UpdateForSaving(FIBITMAP *);
	void ConvertToGreyscale(int);
	void ConvertToGreyscale_lightness();
	void ConvertToGreyscale_luminosity();
	void ConvertToGreyscaleWithDensityArray(int(&result_array)[256], int(&result_array_10)[11], int);
	void equalizeHistogram(int eq_array[], int);
	void set_values(int, int);
	int getHeight();
	int getWidth();
	Pixel getPixel(int, int);
	void setPixel(int, int, Pixel);
	void getRGBChannelSeperatedIntensity(char * filename);
	void getRedChannelOnly();
	void getGreenChannelOnly();
	void getBlueChannelOnly();
	void save_image(FIBITMAP * temp, char *prefix, char *filename);
	unsigned char getRed(int, int);
	unsigned char getGreen(int, int);
	unsigned char getBlue(int, int);
};

