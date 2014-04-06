#include "noise.h"

void Noise::addNoise(int percent){
	if (percent < 0 || percent > 100){
		cout << "Pick a percentage for noise that is acceptable: 0 - 100" << endl;
		return;
	}
	cout << "Applying Noise Filter" << endl;
	int h = getHeight();
	int w = getWidth();
	int allPixels = w*h;
	int ConvertedToNoise = (allPixels / 100)*percent;

	int tempx = 0;
	int tempy = 0;

	Pixel Black;
	Black.RED = 0;
	Black.GREEN = 0;
	Black.BLUE = 0;
	Pixel White;
	White.RED = 255;
	White.GREEN = 255;
	White.BLUE = 255;
	Pixel tempP;
//	Node<char> *p, *q, *r;
	for (int i = 0; i < ConvertedToNoise; i++){
		
		tempx = rand() % w;
		tempy = rand() % h;
		tempP = (i % 2) ? White : Black ;
		this->setPixel(tempx, tempy, tempP);
	}
	cout << "Noise Filter DONE" << endl;
}
void Noise::addNoiseWithNoiseList(int percent, LinkedList<int> &list){
	if (percent < 0 || percent > 100){
		cout << "Pick a percentage for noise that is acceptable: 0 - 100" << endl;
		return;
	}
	cout << "Applying Noise Filter" << endl;
	int h = getHeight();
	int w = getWidth();
	int allPixels = w*h;
	int ConvertedToNoise = (allPixels / 100)*percent;

	int tempx = 0;
	int tempy = 0;

	Pixel Black;
	Black.RED = 0;
	Black.GREEN = 0;
	Black.BLUE = 0;
	Pixel White;
	White.RED = 255;
	White.GREEN = 255;
	White.BLUE = 255;
	Pixel tempP;
	//	Node<char> *p, *q, *r;
	int val = 0;
	for (int i = 0; i < ConvertedToNoise; i++){
		tempx = rand() % w;
		tempy = rand() % h;
		val = (i % 2) ? 255 : 0;
		tempP = (val == 255) ? White : Black;
		list.add(tempx, tempy, val);
		this->setPixel(tempx, tempy, tempP);
	}
	cout << "Noise Filter completed" << endl;
}