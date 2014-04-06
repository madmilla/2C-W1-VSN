#ifndef median_H
#define median_H

#include "Image.h";
#include "linkedList.h";

class Median : public Image{
private:
	int max_pixels = 0;
	int noise_pixels = 0;
public:
	Pixel ** MedianData;
	Median();
	Median(Image * img) : Image(*img){}
	Median(const Median& rhs){}
	Median(const Image& rhs);
	~Median();
	void MedianFilterBW(int sizeOfMask);
	void MedianFilterRGB(int sizeOfMask);
	void checkSuccesAgainstNoiseList(LinkedList<int> &list);
};
#endif