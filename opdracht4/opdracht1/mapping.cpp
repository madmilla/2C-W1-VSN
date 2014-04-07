#include "mapping.h"
#include <math.h> 

Mapping::Mapping(const Image& rhs) : Image(&rhs){
	int w = rhs.getWidth();
	int h = rhs.getHeight();
	MappingData = new Pixel*[w];
	for (int i = 0; i < w; i++)
	{
		MappingData[i] = new Pixel[h];
	}
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			MappingData[i][j].RED = rhs.getRed(i, j);
			MappingData[i][j].GREEN = rhs.getGreen(i, j);
			MappingData[i][j].BLUE = rhs.getBlue(i, j);
		}
	}
}
Mapping::~Mapping(){
	int w = getWidth();
	for (int i = 0; i < w; i++)
	{
		delete[] MappingData[i]; // Deallocate each row.
	}
	delete[] MappingData;
}
void Mapping::transformMapping(double * matrix, int interpolatie = 0){
	cout << "START TRANSFORM MAPPING." << endl;
	double scale_x = matrix[0];
	double scale_y = matrix[4];
	double angle = matrix[1];
	
	double move_x = matrix[2];
	double move_y = matrix[5];
	double width = getWidth();
	double height = getHeight();

	double a0 = cos(angle*PI / 180.0);
	double b0 = sin(angle*PI / 180.0);

	double al = -b0, bl = a0;
	double rx, ry;
	int hw = width / 2;
	int hh = height / 2;
	Pixel p;
	// Prep the image
	Pixel q;
	q.RED = 0;
	q.GREEN = 0;
	q.BLUE = 0;
	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; ++x) {
			this->setPixel(x, y, q);
		}
	}


	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; ++x) {
			rx = a0*x + al*y;
			ry = b0*x + bl*y;
			rx += move_x;
			rx /= scale_x;
			ry /= scale_y;
			ry -= move_y;
			if (rx >= 0 && rx < width && ry >= 0 && ry < height){

				
				if (interpolatie == 1){
					// 1 
					int x0 = floor(rx);
					int x1 = ceil(rx);
					int y0 = floor(ry);
					int y1 = ceil(ry);
					p.RED = (MappingData[x0][y0].RED + MappingData[x0][y1].RED + MappingData[x1][y1].RED + MappingData[x1][y0].RED) / 4;
					p.GREEN = (MappingData[x0][y0].GREEN + MappingData[x0][y1].GREEN + MappingData[x1][y1].GREEN + MappingData[x1][y0].GREEN) / 4;
					p.BLUE = (MappingData[x0][y0].BLUE + MappingData[x0][y1].BLUE + MappingData[x1][y1].BLUE + MappingData[x1][y0].BLUE) / 4;

				}else if (interpolatie == 0){
					// 0
					int x0 = round(rx);
					int y0 = round(ry);
					p.RED = MappingData[x0][y0].RED;
					p.GREEN = MappingData[x0][y0].GREEN;
					p.BLUE = MappingData[x0][y0].BLUE;
				}
				else {
					cout << "This setting for interpolation is unknown." << endl;
				}
				this->setPixel(x, y, p);
			}
			
		}
	}
	cout << "TRANSFORM MAPPING COMPLETED." << endl;
}
