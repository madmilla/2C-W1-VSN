#ifndef thres_H
#define thres_H

#include "Image.h";
#include "linkedList.h";

//@author Lars Veenendaal
//V0.1 03-03-14

class Thresholding : public Image{
private:
	int thresholding = 0;
	int noise_pixels = 0;
public:
	Thresholding();
	Thresholding(Image img) : Image(img){};
	void iterativeThresholding();
};
#endif