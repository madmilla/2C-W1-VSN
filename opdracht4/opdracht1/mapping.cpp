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
void Mapping::transformMapping(){

	int width = getWidth();
	int height = getHeight();

	double aO = Math.cos(angle*Math.PI / 180.0);
	double bO = Math.sin(angle*Math.PI / 180.0);
	double al = -bO, bl = aO;
	int rx, ry;
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x) {
			rx = (int)Math.round(a0*x + al*y);
			ry = (int)Math.round(b0*x + bl*y);
			if (rx >= 0 && rx < width && ry >= 0 && ry < height)
				output.setRGB(rx, ry, input.getRGB(x, y));
		}
	return output;
}
