#ifndef mapping_H
#define mapping_H

#include "Image.h";
#include "linkedList.h";
#define PI 3.14159265

//@author Lars Veenendaal
//V0.1 06-04-14

class Mapping : public Image{
private:
	int max_pixels = 0;
	int noise_pixels = 0;
public:
	Pixel ** MappingData;
	Mapping();
//	Mapping(Image img) : Image(img){};
	Mapping(const Image& rhs);
	~Mapping();
	void transformMapping(double * matrix, int interpolatie);
	//~Noise(){
	//	this->~Image();
	//}
};
#endif