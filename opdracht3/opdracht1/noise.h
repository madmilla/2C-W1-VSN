#ifndef noise_H
#define noise_H

#include "Image.h";
#include "linkedList.h";

//@author Lars Veenendaal
//V0.1 03-03-14

class Noise : public Image{
private:
	int max_pixels =	0;
	int noise_pixels =	0;
public:
	Noise();
	Noise(Image img) : Image(img){};
	void addNoise(int percent);
	void addNoiseWithNoiseList(int percent, LinkedList<int> &list);
	//~Noise(){
	//	this->~Image();
	//}
};
#endif