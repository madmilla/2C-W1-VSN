#include "image.h"
Image::Image(Image &img){
	width = img.getWidth();
	height = img.getHeight();

	int x;
	pixelData = new Pixel*[width]; 
	for (int i = 0; i < width; i++) 
	{
		pixelData[i] = new Pixel[height];
	}
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			pixelData[i][j].RED = 0;
			pixelData[i][j].GREEN = 0;
			pixelData[i][j].BLUE = 0;
		}
	}
	int i = 0, j = 0, temp_calc;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			pixelData[i][j].RED = img.getRed(i,j);
			pixelData[i][j].GREEN = img.getGreen(i, j);
			pixelData[i][j].BLUE = img.getBlue(i, j);
		}
	}
//	printf("Constructor by reference \n");
}
Image::Image(const Image *img){
	width = img->getWidth();
	height = img->getHeight();

	int x;
	pixelData = new Pixel*[width];
	for (int i = 0; i < width; i++)
	{
		pixelData[i] = new Pixel[height];
	}
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			pixelData[i][j].RED = 0;
			pixelData[i][j].GREEN = 0;
			pixelData[i][j].BLUE = 0;
		}
	}
	int i = 0, j = 0, temp_calc;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			pixelData[i][j].RED = img->getRed(i, j);
			pixelData[i][j].GREEN = img->getGreen(i, j);
			pixelData[i][j].BLUE = img->getBlue(i, j);
		}
	}
	//	printf("Constructor by reference \n");
}
Image::Image(FIBITMAP * dib){
	height = FreeImage_GetHeight(dib);
	width = FreeImage_GetWidth(dib);
	int x;
	pixelData = new Pixel*[width];
	for (int i = 0; i < width; i++) 
	{
		pixelData[i] = new Pixel[height];
	}
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			pixelData[i][j].RED = 0;
			pixelData[i][j].GREEN = 0;
			pixelData[i][j].BLUE = 0;
		}
	}
	int i = 0, j = 0, temp_calc;
	RGBQUAD temp_color;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			FreeImage_GetPixelColor(dib, i, j, &temp_color);
			pixelData[i][j].RED = temp_color.rgbRed;
			pixelData[i][j].GREEN = temp_color.rgbGreen;
			pixelData[i][j].BLUE = temp_color.rgbBlue;
		}
	}
}
Image::~Image(){
	for (int i = 0; i < width; i++)
	{
		delete[] pixelData[i]; // Deallocate each row.
	}
	delete[] pixelData;
}
Pixel** Image::getPixelData(){
	return pixelData;
}

// Saving
void Image::save_image(FIBITMAP * temp, char *prefix, char *filename){
	temp = UpdateForSaving(temp);

	char *new_filename = (char *)malloc(strlen(prefix) + strlen(filename) + 1 * sizeof(*prefix));
	if (new_filename == NULL){ return; }
	new_filename[0] = '\0';
	strcat(new_filename, prefix);
	strcat(new_filename, filename);

	FreeImage_Save(FreeImage_GetFileType(filename), temp, new_filename);

	free(new_filename);
}
// Copying everything to the FreeImage Bitmap.
FIBITMAP * Image::UpdateForSaving(FIBITMAP * dib){
	int i = 0, j = 0, temp_calc;
	RGBQUAD *temp_color = new RGBQUAD;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			temp_color->rgbRed = pixelData[i][j].RED;
			temp_color->rgbGreen = pixelData[i][j].GREEN;
			temp_color->rgbBlue = pixelData[i][j].BLUE;
			FreeImage_SetPixelColor(dib, i, j, temp_color);
		}
	}
	return dib;
}

void Image::ConvertToGreyscale(int method = 0){
//	int height = getHeight();
//	int width = getWidth();
	Pixel p;
	int i = 0, j = 0, temp_calc;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			switch (method){
				case 1:
					temp_calc = (maximum(getRed(i, j), getGreen(i, j), getBlue(i, j)) + minimum(getRed(i, j), getGreen(i, j), getBlue(i, j))) / 2;
					break;
				case 2:
					temp_calc = ((getRed(i, j) * 0.21) + (getGreen(i, j) * 0.71) + (getBlue(i, j) * 0.07));
					break;
				case 0:
				default:
					temp_calc = (getRed(i, j) + getGreen(i, j) + getBlue(i, j)) / 3;
					break;
			}

			p.RED = temp_calc;
			p.BLUE = temp_calc;
			p.GREEN = temp_calc;
			setPixel(i, j, p);
		}
	}
}

void Image::ConvertToGreyscaleWithDensityArray(int(&result_array)[256], int(&result_array_10)[11], int method = 0){
//	int height = getHeight();
//	int width = getWidth();
	Pixel p;
	int i = 0, j = 0, temp_calc, temp_10;

	for (int array_filler = 0; array_filler <= 255; array_filler++){	result_array[array_filler] = 0;		}
	for (int array_filler = 0; array_filler <= 10; array_filler++){		result_array_10[array_filler] = 0;	}

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			switch (method){
			case 1:
				temp_calc = (maximum(getRed(i, j), getGreen(i, j), getBlue(i, j)) + minimum(getRed(i, j), getGreen(i, j), getBlue(i, j))) / 2;
				break;
			case 2:
				temp_calc = ((getRed(i, j) * 0.21) + (getGreen(i, j) * 0.71) + (getBlue(i, j) * 0.07));
				break;
			case 0:
			default:
				temp_calc = (getRed(i, j) + getGreen(i, j) + getBlue(i, j)) / 3;
				break;
			}

			temp_10 = (temp_calc * 10) / 256;
			result_array_10[temp_10] = result_array_10[temp_10] + 1;
			result_array[temp_calc] = result_array[temp_calc] + 1;

			p.RED = temp_calc;
			p.BLUE = temp_calc;
			p.GREEN = temp_calc;
			setPixel(i, j, p);
		}
	}
}

void Image::equalizeHistogram(int eq_array[], int method = 0){
//	int height = getHeight();
//	int width = FreeImage_GetWidth(image);

	Pixel p;
	int i = 0, j = 0, temp_calc;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			switch (method){
			case 1:
				temp_calc = (maximum(getRed(i, j), getGreen(i, j), getBlue(i, j)) + minimum(getRed(i, j), getGreen(i, j), getBlue(i, j))) / 2;
				break;
			case 2:
				temp_calc = ((getRed(i, j) * 0.21) + (getGreen(i, j) * 0.71) + (getBlue(i, j) * 0.07));
				break;
			case 0:
			default:
				temp_calc = (getRed(i, j) + getGreen(i, j) + getBlue(i, j)) / 3;
				break;
			}
			//temp_calc = (maximum(temp_color->rgbRed, temp_color->rgbGreen, temp_color->rgbBlue) + minimum(temp_color->rgbRed, temp_color->rgbGreen, temp_color->rgbBlue)) / 2;
			p.RED = eq_array[temp_calc];
			p.BLUE = eq_array[temp_calc];
			p.GREEN = eq_array[temp_calc];
			setPixel(i, j, p);
		}
	}
}


void Image::setGreyScalePixel(int row, int col, int val){
	pixelData[row][col].RED = val;
	pixelData[row][col].GREEN = val;
	pixelData[row][col].BLUE = val;
}


void Image::getRedChannelOnly()const{
	int i = 0, j = 0, temp_10 = 0;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			pixelData[i][j].BLUE = 0;
			pixelData[i][j].GREEN = 0;
			pixelData[i][j].RED = pixelData[i][j].RED;
		}
	}
}
void Image::getGreenChannelOnly()const{
	int i = 0, j = 0;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			pixelData[i][j].BLUE = 0;
			pixelData[i][j].GREEN = pixelData[i][j].GREEN;
			pixelData[i][j].RED = 0;
		}
	}
}
void Image::getBlueChannelOnly()const{
	int i = 0, j = 0;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			pixelData[i][j].BLUE = pixelData[i][j].BLUE;
			pixelData[i][j].GREEN = 0;
			pixelData[i][j].RED = 0;
		}
	}
}

int Image::getWidth()const{
	return width;
}
int Image::getHeight()const{
	return height;
}
Pixel Image::getPixel(int row, int col)const{
	return pixelData[row][col];
}
unsigned char Image::getRed(int row, int col) const{
	return pixelData[row][col].RED;
}
unsigned char Image::getGreen(int row, int col)const{
	return pixelData[row][col].GREEN;
}
unsigned char Image::getBlue(int row, int col)const{
	return pixelData[row][col].BLUE;
}

void Image::getRGBChannelSeperatedIntensity(char * filename)const{
	int r[11], g[11], b[11];
	for (int array_filler = 0; array_filler <= 10; array_filler++){
		r[array_filler] = 0;
		g[array_filler] = 0;
		b[array_filler] = 0;
	}
	int i = 0, j = 0, temp_r = 0, temp_g = 0, temp_b = 0;
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			temp_r = (pixelData[i][j].RED * 10) / 256;
			r[temp_r] = r[temp_r] + 1;
			temp_g = (pixelData[i][j].GREEN * 10) / 256;
			g[temp_g] = g[temp_g] + 1;
			temp_b = (pixelData[i][j].GREEN * 10) / 256;
			b[temp_b] = b[temp_b] + 1;
		}
	}
	saveHistogramToCsv(10, r, filename, "_R_10.csv");
	saveHistogramToCsv(10, g, filename, "_G_10.csv");
	saveHistogramToCsv(10, b, filename, "_B_10.csv");
}
unsigned char Image::getGreyScalePixel(int row, int col){
	return pixelData[row][col].RED;
}
void Image::setPixel(int row, int col, Pixel p){
	pixelData[row][col] = p;
}
void Image::setRed(int row, int col, int val){
	pixelData[row][col].RED = val;
}
void Image::setGreen(int row, int col, int val){
	pixelData[row][col].RED = val;
}
void Image::setBlue(int row, int col, int val){
	pixelData[row][col].RED = val;
}