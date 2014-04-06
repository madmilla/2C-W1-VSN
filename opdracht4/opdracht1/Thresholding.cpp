#include "thresholding.h"
#include "linkedList.h"
/*
Compute i i \, the mean grey level of the comer pixels
Compute H2, the mean grey level of all other pixels
Told = 0
Tnew = : (Ml 4" M2)/2
while Tnew ^ Told do
/ i i = mean grey level of pixels for which ƒ(x, y) < r new
	ji2 = mean grey level of pixels for which f(x, y) ^ Tnew
	Told ~Tnew
	Tnew = 1 T - 1 1 2) 1 2
	end while

	*/
void Thresholding::iterativeThresholding(){
	int w = getWidth();
	int h = getHeight();
	int Threshold = 128;
	int old = 0;
	bool stop = false;
	int temp = 0;
	LinkedList<int> list_1;
	LinkedList<int> list_0;
	cout << "START ITERATIVE THRESHOLDING" << endl;
	while (old != Threshold){
		old = Threshold;
		for (int x = 0; x < w; x++){
			for (int y = 0; y < h; y++){
				temp = this->getGreyScalePixel(x, y);
					if(temp >= Threshold){
					
						list_1.add(x, y, temp);
				}
				else{
					list_0.add(x, y, temp);
				}
			}
		}
		cout << "T0 count: " << list_0.count() << " AVG: " << list_0.getAverageWithID(3) << endl;
		cout << "T1 count: " << list_1.count() << " AVG: " << list_1.getAverageWithID(3) << endl;
		Threshold = (list_0.getAverageWithID(3) + list_1.getAverageWithID(3)) / 2;
		cout << "OLD: " << old << " THRES: " << Threshold << endl;
	}
	cout << "ITERATIVE THRESHOLDING COMPLETE" << endl;
	cout << "APPLYING ITERATIVE THRESHOLDING" << endl;
	for (int x = 0; x < w; x++){
		if (x % (w / 10) == 0){
			cout << (((double)x / (double)w) * (double)100) << "%" << endl;
		}
		for (int y = 0; y < h; y++){
			temp = this->getGreyScalePixel(x, y);
			if (temp >= Threshold){
				this->setGreyScalePixel(x,y,255);
			}
			else{
				this->setGreyScalePixel(x, y, 0);
			}
		}
	}
	cout << "APPLYING ITERATIVE THRESHOLDING COMPLETE" << endl;
}
	/*

	int low = 0;
	int high = 255;
		
		Compute i i \, the mean grey level of the comer pixels
		Compute H2, the mean grey level of all other pixels
		Told = 0
		Tnew = : (Ml 4" M2)/2
	while Tnew ^ Told do
	/ i i = mean grey level of pixels for which ƒ(x, y) < r new
		ji2 = mean grey level of pixels for which f(x, y) ^ Tnew
		Told ~Tnew
		Tnew = 1 T - 1 1 2) 1 2
		end while
		}*/