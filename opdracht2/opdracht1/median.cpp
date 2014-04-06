#include "median.h"
#include "linkedList.h"


Median::Median(const Image& rhs) : Image(&rhs){
	int w = rhs.getWidth();
	int h = rhs.getHeight();
	MedianData = new Pixel*[w];
	for (int i = 0; i < w; i++)
	{
		MedianData[i] = new Pixel[h];
	}
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			MedianData[i][j].RED = rhs.getRed(i, j);
			MedianData[i][j].GREEN = rhs.getGreen(i, j);
			MedianData[i][j].BLUE = rhs.getBlue(i, j);
		}
	}
}
Median::~Median(){
	int w = getWidth();
	for (int i = 0; i < w; i++)
	{
		delete[] MedianData[i]; // Deallocate each row.
	}
	delete[] MedianData;
}

void Median::MedianFilterRGB(int sizeOfMask){

	// This is the bogosort of medianfiltering.
	// Havent had much time updating it or rewriting it for the walker.

	// Had this idea of a array with 9 values and whenever it moves it would just kick off 3 values and gather 3 new ones.
	// If i have the time i will make this walker algorithme.

	if (sizeOfMask < 3) {
		cout << "Median mask is too small (Starts from 3)." << endl;
		return;
	}
	else if (sizeOfMask > 11){
		cout << "Median mask is too big (capped to 11)." << endl;
		return;
	}
	if (sizeOfMask % 2 == 0){
		cout << "Median mask must be a odd number." << endl;
		return;
	}
	cout << "Applying Median Filter " << sizeOfMask << "X" << sizeOfMask << endl;
	int h = getHeight();
	int w = getWidth();
	int allPixels = w*h;

	// Prepare the mask.
	int MatrixMin = -(sizeOfMask / 2);
	int MatrixMax = (sizeOfMask / 2);
	int middle = (sizeOfMask * sizeOfMask) / 2;
	for (int Height = 0; Height < h; ++Height){
		if (Height % 100 == 0){

			cout << ((double)Height / (double)h) * (double)100 << "%" << endl;
		}
		for (int Width = 0; Width < w; ++Width){
			// Make ( / redefine) lists for Red, Green, Blue
			// We dont care about alpha
			LinkedList<int> RValues;
			LinkedList<int> GValues;
			LinkedList<int> BValues;

			// Go through every pixel in the mask and get the specific color information and store that information in the lists.
			
			for (int actualPositionX = (Width + MatrixMin); actualPositionX <= (Width + MatrixMax); ++actualPositionX)
			{
				for (int actualPositionY = (Height + MatrixMin); actualPositionY <= (Height + MatrixMax); ++actualPositionY)
				{
					if (Width == 100 && Height == 100){
					//	cout << "APX" << actualPositionX << "APY" << actualPositionY << endl;
						
					}
				//	int actualPositionX = Width + MaskWidth;
				//	int actualPositionY = Height + MaskHeight;
					if (actualPositionX > 0 && actualPositionX < w && actualPositionY > 0 && actualPositionY < h)
					{
							// Fill up the R linkedlists
						RValues.add((int)MedianData[actualPositionX][actualPositionY].RED);
						GValues.add((int)MedianData[actualPositionX][actualPositionY].GREEN);
						BValues.add((int)MedianData[actualPositionX][actualPositionY].BLUE);
					}
				}
			}
			// Seems to be the fastest method of sorting.
			// Quickselect cant touch the speed of the sort.
			// Tried multiple sorting methods there might be faster ones though.
			if (RValues.count() != (sizeOfMask * sizeOfMask)){
			//	cout << "Mask is fout" << RValues.count() << "COMPARE" << sizeOfMask*sizeOfMask << " W " << Width << " H " << Height << endl;
			}
			else{
				RValues.sort(0, RValues.count());
				GValues.sort(0, GValues.count());
				BValues.sort(0, BValues.count());

				Pixel MEDIAN;
				MEDIAN.RED = RValues.getValueBasedOnNumberInLine(middle);
				MEDIAN.GREEN = GValues.getValueBasedOnNumberInLine(middle);
				MEDIAN.BLUE = BValues.getValueBasedOnNumberInLine(middle);


				this->setPixel(Width, Height, MEDIAN);
				
			}
			//RValues.print();
		}
		

	}
	cout << "Applying Median Filter completed" << endl;
}

void Median::checkSuccesAgainstNoiseList(LinkedList<int> &list){
	cout << "Checking the succes of reducing the noise with median." << endl;
	int i = list.count();
	int j = 1;
	int fout = 0;
	while (j < i){
	//	cout << list.getValueBasedOnNumberInLine(j) << " - " << (int)this->getRed(list.getDataXBasedOnNumberInLine(j), list.getDataYBasedOnNumberInLine(j)) << endl;
	//	cout << "(" << list.getDataXBasedOnNumberInLine(j) << "," << list.getDataXBasedOnNumberInLine(j) << ")" << endl;
	//	cout << this->getRed(list.getDataXBasedOnNumberInLine(j), list.getDataXBasedOnNumberInLine(j)) << endl;
	if (list.getValueBasedOnNumberInLine(j) == (int)this->getRed(list.getDataXBasedOnNumberInLine(j), list.getDataYBasedOnNumberInLine(j))){
			fout++;
		}
		j++;
	}
	cout << "Succesrate of the median:\nNoise added: " << i << "\nAfter the median:" << fout << "\nFailrate:" << (double)(((double)fout / (double)i) * 100) << " procent." << endl;
}