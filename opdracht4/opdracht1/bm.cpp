#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "bm.h"

using namespace std;

/**
* bm.c
* For some reason i though this was a good idea.
*
* DIB = Device independant Bitmap	http://msdn.microsoft.com/en-us/library/windows/desktop/dd183562(v=vs.85).aspx
* BMP = Bitmap						http://msdn.microsoft.com/en-us/library/windows/desktop/dd183371(v=vs.85).aspx
* 
* libbmp - BMP library
*
*/


/*
* In this lib i reconstruct the structure of the bitmap
* see - http://upload.wikimedia.org/wikipedia/commons/c/c4/BMPfileFormat.png
*
* BMP File Header  Stores general information about the BMP file.
* DIB header       Stores detailed information about the bitmap image.
* Color Palette    Stores the definition of the colors being used for
*                    indexed color bitmaps.
* Bitmap Data      Stores the actual image, pixel by pixel.
*/

#define DEFAULT_DPI_X 3780 // MSDN calls these the magic numbers
#define DEFAULT_DPI_Y 3780 // MSDN magic numbers
#define DPI_FACTOR 39.37007874015748

/*
Create a grayscale version of the bmp
*/

static void bmp_create_grayscale(bmpFile *bmp){
	int i;
	unsigned int step_size, value;

	if (!bmp->colors){
		return;
	}
	if (bmp->dib.bits_per_pixel != 1){
		step_size = 255 / (bmp->dib.nr_colors - 1);
	}
	else{
		step_size = 255;
	}
	
	for (i = 0; i <= bmp->dib.nr_colors; ++i){
		value = i * step_size;
		rgb_pixel color = { value, value, value, 0 };
		bmp->colors[i] = color;
	}
}

/*
Getters
*/

unsigned int bmp_get_width(bmpFile *bmp){
	return bmp->dib.width;
}

unsigned int bmp_get_height(bmpFile *bmp){
	return bmp->dib.height;
}

unsigned int bmp_get_depth(bmpFile *bmp){
	return bmp->dib.bits_per_pixel;
}

rgb_pixel *bmp_get_pixel(bmpFile *bmp, unsigned int x, unsigned int y){
	// If the values are outside of the size return NULL
	if ((x >= bmp->dib.width) || (y >= bmp->dib.height)){		
		return NULL;
	}
	else{
		return &(bmp->pixels[x][y]);
	}
}

bool bmp_set_pixel(bmpFile *bmp, unsigned int x, unsigned int y, rgb_pixel pixel){
	// If the values are outside of the size return NULL
	if ((x >= bmp->dib.width) || (y >= bmp->dib.height)){
		return false;
	}
	else{
		bmp->pixels[x][y] = pixel;
		return true;
	}
}

bool SafeFread(char* buffer, int size, int number, FILE* fp)
{
	/*	Reads an array of a number of elements, 
		each one with a 'size' of size bytes from the filepointer,
		and stores them in the block of memory specified by buffer.
	*/

	int ItemsRead;
	if (feof(fp))
	{
		return false;
	}
	ItemsRead = (int)fread(buffer, size, number, fp);
	if (ItemsRead < number)
	{
		return false;
	}
	return true;
}

bmpFile * bmp_open(const char *fileName){
	using namespace std;
	FILE* fp;
	bmpFile *bmap;


	int row;
	unsigned char *buffer;
	if ((fp = fopen(fileName, "rb")) == NULL){
		// The care is low in this one.
		return false;
	}

	bmap = (bmpFile *)malloc(sizeof(bmpFile));
	memset(bmap, 0, sizeof(bmpFile));
	//memset(bmp, 0, sizeof(bmpFile));

	bool noError = true; // if everything goes right this will stay true and everything went ok.
	noError &= SafeFread((char*)&(bmap->header.signature), sizeof(unsigned short), 1, fp);
	//Fill up the header if possibru
	noError &= SafeFread((char*)&(bmap->header.filesize), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->header.reserved1), sizeof(unsigned short), 1, fp);
	noError &= SafeFread((char*)&(bmap->header.reserved2), sizeof(unsigned short), 1, fp);
	noError &= SafeFread((char*)&(bmap->header.offset), sizeof(unsigned int), 1, fp);
		//Fill up the dib header
	noError &= SafeFread((char*)&(bmap->dib.header_size), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.width), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.height), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.nr_planes), sizeof(unsigned short), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.bits_per_pixel), sizeof(unsigned short), 1, fp);

	noError &= SafeFread((char*)&(bmap->dib.compress_type), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.bmp_bytesize), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.xPixelsPerMeter), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.yPixelsPerMeter), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.nr_colors), sizeof(unsigned int), 1, fp);
	noError &= SafeFread((char*)&(bmap->dib.nr_imp_colors), sizeof(unsigned int), 1, fp);

	//cout << "\nSG " << &(bmap->header.signature) << "\nFS " << bmap->header.filesize << "\nRS1 " << bmap->header.reserved1 << "\nRS2 " << bmap->header.reserved2 << "\nOFF " << bmap->header.offset << endl;

	bmap->header.signature[0] = 'B';
	bmap->header.signature[1] = 'M';

	cout << "biSize: " << (int)bmap->dib.header_size << endl
		<< "biWidth: " << (int)bmap->dib.width << endl
		<< "biHeight: " << (int)bmap->dib.height << endl
		<< "biPlanes: " << (int)bmap->dib.nr_planes << endl
		<< "biBitCount: " << (int)bmap->dib.bits_per_pixel << endl
		<< "biCompression: " << (int)bmap->dib.compress_type << endl
		<< "biSizeImage: " << (int)bmap->dib.bmp_bytesize << endl
		<< "biXPelsPerMeter: " << (int)bmap->dib.xPixelsPerMeter << endl
		<< "biYPelsPerMeter: " << (int)bmap->dib.yPixelsPerMeter << endl
		<< "biClrUsed: " << (int)bmap->dib.nr_colors << endl
		<< "biClrImportant: " << (int)bmap->dib.nr_imp_colors << endl << endl;

	cout << "bfType: " << (int)bmap->header.signature << endl
		<< "bfSize: " << (int)bmap->header.filesize << endl
		<< "bfReserved1: " << (int)bmap->header.reserved1 << endl
		<< "bfReserved2: " << (int)bmap->header.reserved2 << endl
		<< "bfOffBits: " << (int)bmap->header.offset << endl << endl;


//	bmap->dib.xPixelsPerMeter = DEFAULT_DPI_X;
//	bmap->dib.yPixelsPerMeter = DEFAULT_DPI_Y;

	if (bmap->dib.bits_per_pixel == 16){
		bmap->dib.compress_type = BI_BITFIELDS;
	}
	else {
		bmap->dib.compress_type = BI_RGB;
	}



	unsigned int xPixels = bmap->dib.xPixelsPerMeter;
	unsigned int yPixels = bmap->dib.yPixelsPerMeter;

	int TempBpp = (int)bmap->dib.bits_per_pixel;


	// Fish for errors;
	if (!noError){
		// its all tits up, abort.
		cout << "1" << endl;
		fclose(fp);
		return false;
	}
	if (bmap->dib.compress_type == 1 || bmap->dib.compress_type == 2 || bmap->dib.compress_type > 3 || (bmap->dib.compress_type == 3 && bmap->dib.bits_per_pixel != 16)){
		// 1 & 2 is RLE, 3 is OS2 ether way i cant deal with it :(
		// read somewhere there were higher ones. Again cant deal with it.
		cout << "2" << endl;
		fclose(fp);
		return false;
	}

	if (TempBpp != 1 && TempBpp != 4 && TempBpp != 8 && TempBpp != 16 && TempBpp != 24 && TempBpp != 32){
		// if its not standard i dont know what to do.
		cout << "3" << endl;
		fclose(fp);
		return false;
	}
	bmp_malloc_pixels_from_bmp(bmap);
	bmp_malloc_colors(bmap);
	bmap->colors = new rgb_pixel [bmap->dib.nr_colors];
	int colorCount = 0;
	int colorCountShouldBe = 2;

	int output = unsignedint_pow(2, bmap->dib.bits_per_pixel);
	if (bmap->dib.bits_per_pixel == 32)
	{
		colorCountShouldBe = unsignedint_pow(2, 24);
	}
	

	for (colorCount = 0; colorCount < colorCountShouldBe; colorCount++)
	{
		bmap->colors[colorCount].red = colorCount * 255;
		bmap->colors[colorCount].green = colorCount * 255;
		bmap->colors[colorCount].blue = colorCount * 255;
		bmap->colors[colorCount].alpha = 0;
		cout << "R " << bmap->colors[colorCount].red << " G " << bmap->colors[colorCount].green << " B " << bmap->colors[colorCount].blue << endl;
	}

	// preliminaries

	double dBytesPerPixel = ((double)bmap->dib.bits_per_pixel) / 8.0;
	double dBytesPerRow = dBytesPerPixel * (bmap->dib.width + 0.0);
	dBytesPerRow = ceil(dBytesPerRow);

	int BytePaddingPerRow = 4 - ((int)(dBytesPerRow)) % 4;
	if (BytePaddingPerRow == 4){ BytePaddingPerRow = 0; }

	// bpp: < 16 - Read the palette
	if (bmap->dib.bits_per_pixel < 16){
		int NrColorsFound = ((int)bmap->header.offset - 54) / 4;
		if (NrColorsFound > unsignedint_pow(2, bmap->dib.bits_per_pixel)){
			NrColorsFound = unsignedint_pow(2, bmap->dib.bits_per_pixel); // if its incorrect smooth it out.
		}
		int i;
		for (i = 0; i < NrColorsFound; i++){
			
			SafeFread((char*)&(bmap->colors[i]), 4, 1, fp);
			
		}
		for (i = NrColorsFound; i < TellNumberOfColors(bmap->dib.bits_per_pixel); i++){
			//In case there are colors that are not found just add white as fix.
			bmap->colors[i].red = 255;
			bmap->colors[i].green = 255;
			bmap->colors[i].blue = 255;
			bmap->colors[i].alpha = 0;
		}
	
	}
	
	// Skip blank data 
	int bytesToSkip = bmap->header.offset - 54;
	if (bmap->dib.bits_per_pixel < 16){
		bytesToSkip -= 4 * unsignedint_pow(2, bmap->dib.bits_per_pixel);
	}
	if (bmap->dib.bits_per_pixel == 16 && bmap->dib.compress_type == 3){
		bytesToSkip -= 3 * 4; // EASY BMP waarde
	}
	if (bytesToSkip < 0){ bytesToSkip = 0; } //
	if (bytesToSkip > 0 && bmap->dib.bits_per_pixel != 16){
		// Something goofed find the remaining bytestoskip and skip them.
		unsigned char* wastedSpace;
		wastedSpace = new unsigned char[bytesToSkip];
		SafeFread((char*)wastedSpace, bytesToSkip, 1, fp);
		delete[] wastedSpace;
	}

	// The following reads 1, 4, 8, 24 and 32 bpp files
	// We can buffer these which is much nicer than the current 16 bpp version.
	int i, j;
	if (bmap->dib.bits_per_pixel != 16){
		int bufferSize = (int)((bmap->dib.width*bmap->dib.bits_per_pixel) / 8.0);
		while (8 * bufferSize < bmap->dib.width * bmap->dib.bits_per_pixel){
			bufferSize++;
		}
		while (bufferSize % 4){
			bufferSize++;
		}
		unsigned char* buffer;
		buffer = new unsigned char [bufferSize];
		j = bmap->dib.height - 1;
		while (j > -1){
			int bytesRead = (int)fread((char*)buffer, 1, bufferSize, fp);
			if (bytesRead < bufferSize){
				j = -1;
				cout << "The size is off :S" << endl;
			}
			else{
				bool succes = false;
				
			  	if (bmap->dib.bits_per_pixel == 1){ succes = bmp_get_row_data_for_1(bmap, buffer, bufferSize, j); }
				if (bmap->dib.bits_per_pixel == 4){ succes = bmp_get_row_data_for_4(bmap, buffer, bufferSize, j); }
				if (bmap->dib.bits_per_pixel == 8){ succes = bmp_get_row_data_for_8(bmap, buffer, bufferSize, j); }
				if (bmap->dib.bits_per_pixel == 24){ succes = bmp_get_row_data_for_24(bmap, buffer, bufferSize, j); }
				if (bmap->dib.bits_per_pixel == 32){ succes = bmp_get_row_data_for_32(bmap, buffer, bufferSize, j); }
				if (!succes){
					cout << "Could not read bis per pixel data!" << endl;
					j = -1;
				}
				//fwrite(buf, bytes_per_line, 1, fp);
			}
			j--;
		}
		delete [] buffer;
	}
	if (bmap->dib.bits_per_pixel == 16){
		int dataBytes = bmap->dib.width * 2;
		int paddingBytes = (4 - dataBytes % 4) % 4;

		// Set default mask

		unsigned short blueMask = 31;
		unsigned short greenMask = 992;
		unsigned short redMask = 31744;

		// read the bit fields, if necessary to
		// override the default 5-5-5 mask

		if (bmap->dib.compress_type != 0){
			
			// read the three bit masks 
			unsigned short tempMask;
			unsigned short zero;

			SafeFread((char *)&redMask, 2, 1, fp);
			SafeFread((char *)&tempMask, 2, 1, fp);

			SafeFread((char *)&greenMask, 2, 1, fp);
			SafeFread((char *)&tempMask, 2, 1, fp);

			SafeFread((char *)&blueMask, 2, 1, fp);
			SafeFread((char *)&tempMask, 2, 1, fp);

		}

		// read and skip any meta data

		if (bytesToSkip > 0){
			// Extra data gevonden.

			unsigned char* tempSkip;
			tempSkip = new unsigned char[bytesToSkip];
			SafeFread((char *)tempSkip, bytesToSkip, 1, fp);
			delete[] tempSkip;
		}

		// Determine the red, green and blue shifts

		int greenShift = 0;
		int blueShift = 0;
		int redShift = 0;
		unsigned short tempShift;
		tempShift = greenMask;
		while (tempShift > 31){	tempShift = tempShift >> 1, greenShift++; }
		tempShift = blueShift;
		while (tempShift > 31){ tempShift = tempShift >> 1, blueShift++; }
		tempShift = redShift;
		while (tempShift > 31){ tempShift = tempShift >> 1, redShift++; }

		// read the actual pixels
		for (j = bmap->dib.height - 1; j >= 0; j--){
			i = 0;
			int readNumber = 0;
			while (readNumber < dataBytes){
				unsigned short temp;
				SafeFread((char*)&temp, 2, 1, fp);
				readNumber += 2;

				unsigned short red = redMask & temp;
				unsigned short green = greenMask & temp;
				unsigned short blue = blueMask & temp;

				unsigned char blueByte = (unsigned char)8 * (blue >> blueShift);
				unsigned char greenByte = (unsigned char)8 * (green >> greenShift);
				unsigned char redByte = (unsigned char)8 * (red >> redShift);

				bmap->pixels[i][j].red = redByte;
				bmap->pixels[i][j].green = greenByte;
				bmap->pixels[i][j].blue = blueByte;

				i++;
			}
			readNumber = 0;
			while (readNumber < paddingBytes){
				unsigned char tempByte;
				SafeFread((char *)&tempByte, 1, 1, fp);
				readNumber++;
			}
		}
	}

	cout << " ARTORIAS " << endl;
	
	fclose(fp);
	return bmap;
}
int TellNumberOfColors(unsigned int bpp)
{
	int output = unsignedint_pow(2, bpp);
	if (bpp == 32)
	{
		output = unsignedint_pow(2, 24);
	}
	return output;
}

bool bmp_save(bmpFile *bmp, const char *filename){
	FILE *fp;
	int row;
	unsigned char *buffer;

	/* Create the file */
	if ((fp = fopen(filename, "wb")) == NULL){
		return false;
	}

	/* write data to the file */
	bmp_write_header(bmp, fp);	//Fills the neccesary fields.
	bmp_write_dib(bmp, fp);		//Fills the neccesary device independant bitmap.
	bmp_write_palette(bmp, fp); //Fills the palette or bitmask.

	if (bmp->dib.bits_per_pixel == 16){
		unsigned int data_bytes = bmp->dib.width * 2;
		unsigned int padding_bytes = 4 - data_bytes % 4;

		for (row = bmp->dib.height - 1; row >= 0; --row){
			unsigned char zero_byte = 0;
			unsigned int write_number = 0;

			for (int i = 0; write_number < data_bytes; ++i, write_number += 2){
				cout << "16 BPP R " << bmp->pixels[i][row].red << " G " << bmp->pixels[i][row].green << " B " << bmp->pixels[i][row].blue << endl;
				unsigned short red = (unsigned int)(bmp->pixels[i][row].red / 8);
				unsigned short green = (unsigned int)(bmp->pixels[i][row].green / 4);
				unsigned short blue = (unsigned int)(bmp->pixels[i][row].blue / 8);
				unsigned short value = (red << 11) + (green << 5) + blue;
			}

			for (write_number = 0; write_number < padding_bytes; ++write_number){
				fwrite(&zero_byte, 1, 1, fp);
			}
		}
	}
	else{
		double bytes_per_pixel;
		int bytes_per_line;

		bytes_per_pixel = (bmp->dib.bits_per_pixel * 1.0) / 8.0;
		bytes_per_line = (int)ceil(bytes_per_pixel * bmp->dib.width);
		if (bytes_per_line % 4 != 0){
			bytes_per_line += 4 - bytes_per_line % 4; // Make it equal
		}
		buffer = (unsigned char * )malloc(bytes_per_line);

		for (row = bmp->dib.height - 1; row >= 0; --row){

			memset(buffer, 0, bytes_per_line);
			
			// Definitly want to change this for a function pointer.
			switch (bmp->dib.bits_per_pixel){
			case 1:
				bmp_get_row_data_for_1(bmp, buffer, bytes_per_line, row);
				break;
			case 4:
				bmp_get_row_data_for_4(bmp, buffer, bytes_per_line, row);
				break;
			case 8:
				bmp_get_row_data_for_8(bmp, buffer, bytes_per_line, row);
				break;
			case 24:
				bmp_get_row_data_for_24(bmp, buffer, bytes_per_line, row);
				break;
			case 32:
				bmp_get_row_data_for_32(bmp, buffer, bytes_per_line, row);
				break;
			}

			fwrite(buffer, bytes_per_line, 1, fp);
		}
		free(buffer);

	}


	fclose(fp);
	return true;
}

static void bmp_write_header(bmpFile *bmp, FILE *fp){
	bmp_header header = bmp->header;
	//cout << "SG3 " << &(header.signature) << "\nFS " << &(header.filesize) << "\nRS1 " << &(header.reserved1) << "\nRS2 " << &(header.reserved2) << "\nOFF " << &(header.offset) << endl;
	fwrite(&(header.signature), sizeof(header.signature), 1, fp);
	fwrite(&(header.filesize), sizeof(unsigned int), 1, fp);
	fwrite(&(header.reserved1), sizeof(unsigned short), 1, fp);
	fwrite(&(header.reserved2), sizeof(unsigned short), 1, fp);
	fwrite(&(header.offset), sizeof(unsigned int), 1, fp);
}

static void bmp_write_dib(bmpFile *bmp, FILE *fp){
	bmp_dib_header dib = bmp->dib;
//	cout << "HS " << dib.header_size << "\nW " << dib.width << "\nH " << dib.height << "\nNRP " << dib.nr_planes << "\nBPP " << dib.bits_per_pixel << endl;
//	cout << "CT " << dib.compress_type << "\nBS " << dib.bmp_bytesize << "\nxPPM " << dib.xPixelsPerMeter << "\nyPPM " << dib.yPixelsPerMeter << "\nNRC " << dib.nr_colors << "\nNRIC " << dib.nr_imp_colors << endl;
	fwrite(&(dib.header_size), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.width), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.height), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_planes), sizeof(unsigned short), 1, fp);
	fwrite(&(dib.bits_per_pixel), sizeof(unsigned short), 1, fp);
	fwrite(&(dib.compress_type), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.bmp_bytesize), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.xPixelsPerMeter), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.yPixelsPerMeter), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_colors), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_imp_colors), sizeof(unsigned int), 1, fp);
}

static void bmp_write_palette(bmpFile *bmp, FILE *fp){
	cout << "BPP " << bmp->dib.bits_per_pixel << endl;

	if (bmp->dib.bits_per_pixel == 1 || bmp->dib.bits_per_pixel == 4 || bmp->dib.bits_per_pixel == 8){
		for (int i = 0; i < bmp->dib.nr_colors; ++i){
			fwrite(&(bmp->colors[i]), sizeof(rgb_pixel), 1, fp);
			cout << "Colors " << bmp->colors[i].red << endl;
		}
	}
	else if (bmp->dib.bits_per_pixel == 16){
		//16 and up are bit masks not palettes
		unsigned short red_mask = 63488;	// 1111 1000 0000 0000
		unsigned short green_mask = 2016;	// 0000 0111 1110 0000 
		unsigned short blue_mask = 31;		// 0000 0000 0001 1111
		unsigned short zero = 0;			// 0000 0000 0000 0000

		fwrite(&red_mask, sizeof(unsigned short), 1, fp);
		fwrite(&zero, sizeof(unsigned short), 1, fp);
		fwrite(&green_mask, sizeof(unsigned short), 1, fp);
		fwrite(&zero, sizeof(unsigned short), 1, fp);
		fwrite(&blue_mask, sizeof(unsigned short), 1, fp);
		fwrite(&zero, sizeof(unsigned short), 1, fp);
	}
}

bmpFile * bmp_create(unsigned int width, unsigned int height, unsigned int bits_per_pixel){
	bmpFile *result;
	double bytes_per_pixel;
	unsigned int bytes_per_line;
	unsigned int palette_size;

	if (bits_per_pixel != 1 && bits_per_pixel != 4 && bits_per_pixel != 8 && bits_per_pixel != 16 && bits_per_pixel != 24 && bits_per_pixel != 32){
		return NULL;
	}
	result = (bmpFile * )malloc(sizeof(bmpFile));
	memset(result, 0, sizeof(bmpFile));

	result->header.signature[0] = 'B';
	result->header.signature[1] = 'M';
	
	result->dib.header_size = 40;
	result->dib.width = width;
	result->dib.height = height;
	result->dib.nr_planes = 1;
	result->dib.bits_per_pixel = bits_per_pixel;
	result->dib.xPixelsPerMeter = DEFAULT_DPI_X;
	result->dib.yPixelsPerMeter = DEFAULT_DPI_Y;

	if (bits_per_pixel == 16){
		result->dib.compress_type = BI_BITFIELDS;
	}
	else {
		result->dib.compress_type = BI_RGB;
	}

	bmp_malloc_pixels_from_bmp(result);
	bmp_malloc_colors(result);

	/* Calculate the field value of header and the dib */
	bytes_per_pixel = (result->dib.bits_per_pixel * 1.0) / 8.0;
	bytes_per_line = (int)ceil(bytes_per_pixel * result->dib.width);
	if (bytes_per_line % 4 != 0){
		bytes_per_line += 4 - bytes_per_line % 4;
	}

	result->dib.bmp_bytesize = bytes_per_line * result->dib.height;

	palette_size = 0;
	if (bits_per_pixel == 1 || bits_per_pixel == 4 || bits_per_pixel == 8){
		palette_size = unsignedint_pow(2, result->dib.bits_per_pixel) * 4;
	}
	else if (result->dib.bits_per_pixel == 16){
		palette_size = 3 * 4;
	}

	result->header.offset = 14 + result->dib.header_size + palette_size;
	result->header.filesize = result->header.offset + result->dib.bmp_bytesize;

	return result;

}

static void bmp_malloc_pixels_from_bmp(bmpFile *bmp){
	int i, j;

	bmp->pixels = (rgb_pixel **)malloc(sizeof(rgb_pixel *)* bmp->dib.width);
	
	for (i = 0; i < bmp->dib.width; ++i){
		bmp->pixels[0 + i] = (rgb_pixel *)malloc(sizeof(rgb_pixel)* bmp->dib.height);
		for (j = 0; j < bmp->dib.height; ++j){

			bmp->pixels[i][j].red = 255;
			bmp->pixels[i][j].green = 255;
			bmp->pixels[i][j].blue = 255;
			bmp->pixels[i][j].alpha = 0;
		}
	}
}

static void bmp_malloc_colors(bmpFile *bmp){
	bmp->dib.nr_colors = unsignedint_pow(2, bmp->dib.bits_per_pixel);
	if (bmp->dib.bits_per_pixel == 1 || bmp->dib.bits_per_pixel == 4 || bmp->dib.bits_per_pixel == 8){
		bmp->colors = (rgb_pixel *)malloc(sizeof(rgb_pixel)* bmp->dib.nr_colors);
		bmp_create_standard_color_table(bmp);
	}
}

static void bmp_create_standard_color_table(bmpFile *bmp){
	int i, red_counter, green_counter, blue_counter;

	switch (bmp->dib.bits_per_pixel){
	case 1:
		for (i = 0; i < 2; ++i){
			bmp->colors[i].red = i * 255;
			bmp->colors[i].green = i * 255;
			bmp->colors[i].blue = 1 * 255;
			bmp->colors[i].alpha = 0;
		}
		break;
	case 4:
		i = 0;
		for (blue_counter = 0; blue_counter < 2; ++blue_counter){
			for (green_counter = 0; green_counter < 2; ++green_counter){
				for (red_counter = 0; red_counter < 2; ++red_counter){
					bmp->colors[i].red = red_counter * 128;
					bmp->colors[i].green = green_counter * 128;
					bmp->colors[i].blue = blue_counter * 128;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}
		for (blue_counter = 0; blue_counter < 2; ++blue_counter){
			for (green_counter = 0; green_counter < 2; ++green_counter){
				for (red_counter = 0; red_counter < 2; ++red_counter){
					bmp->colors[i].red = red_counter * 255;
					bmp->colors[i].green = green_counter * 255;
					bmp->colors[i].blue = blue_counter * 255;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}
		i = 8;
		bmp->colors[i].red = 192;
		bmp->colors[i].green = 192;
		bmp->colors[i].blue = 192;
		bmp->colors[i].alpha = 0;
		break;
	case 8:
		i = 0;
		for (blue_counter = 0; blue_counter < 4; ++blue_counter){
			for (green_counter = 0; green_counter < 8; ++green_counter){
				for (red_counter = 0; red_counter < 8; ++red_counter){
					bmp->colors[i].red = red_counter * 32;
					bmp->colors[i].green = green_counter * 32;
					bmp->colors[i].blue = blue_counter * 64;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}
		i = 0;
		for (blue_counter = 0; blue_counter < 2; ++blue_counter){
			for (green_counter = 0; green_counter < 2; ++green_counter){
				for (red_counter = 0; red_counter < 2; ++red_counter){
					bmp->colors[i].red = red_counter * 128;
					bmp->colors[i].green = green_counter * 128;
					bmp->colors[i].blue = blue_counter * 128;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}
		break;
	}
}

static void bmp_create_grayscale_color_table(bmpFile *bmp){
	int i;
	unsigned char step_size;
	if (!bmp->colors){
		return;
	}
	if (bmp->dib.bits_per_pixel != 1){
		step_size = 255 / (bmp->dib.nr_colors - 1);
	}
	else{
		step_size = 255;
	}

	for (i = 0; i < bmp->dib.nr_colors; ++i){
		unsigned char value = i * step_size;
		rgb_pixel color = { value, value, value, 0 };
		bmp->colors[i] = color;
	}
}

static unsigned int unsignedint_pow(unsigned int base, unsigned int bits_per_pixel){
	unsigned i, result = 1;
	
	for (i = 0; i < bits_per_pixel; ++i){
		result *= base;
	}
	return result;
}

/*
Get row data 
*/
static bool bmp_get_row_data_for_1(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	unsigned char pos_weight[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	unsigned int i = 0, j, k = 0;
	unsigned int index;

	if (bmp->dib.width > 8 * buffer_length){
		return false;
	}

	while (i < bmp->dib.width){
		for (j = 0, index = 0; j < 8 && i < bmp->dib.width; ++i, ++j){
			index += pos_weight[j] * find_closest_color(bmp, bmp->pixels[i][row]);
		}
		buffer[k++] = index & 0xff;
	}
	
	return true;
}
//////////////////////////////////
/*
rgb_pixel GetColor(int ColorNumber)
{
	rgb_pixel Output;
	Output.red = 255;
	Output.green = 255;
	Output.blue = 255;
	Output.alpha = 0;

	using namespace std;
	if (BitDepth != 1 && BitDepth != 4 && BitDepth != 8)
	{
			cout << "EasyBMP Warning: Attempted to access color table for a BMP object" << endl
				<< "                 that lacks a color table. Ignoring request." << endl;
		return Output;
	}
	if (!colors)
	{
			cout << "EasyBMP Warning: Requested a color, but the color table" << endl
				<< "                 is not defined. Ignoring request." << endl;
		return Output;
	}
	if (ColorNumber >= TellNumberOfColors())
	{
			cout << "EasyBMP Warning: Requested color number "
				<< ColorNumber << " is outside the allowed" << endl
				<< "                 range [0," << TellNumberOfColors() - 1
				<< "]. Ignoring request to get this color." << endl;
		return Output;
	}
	Output = Colors[ColorNumber];
	return Output;
}


RGBApixel* BMP::operator()(int i, int j)
{
	using namespace std;
	bool Warn = false;
	if (i >= Width){i = Width - 1; Warn = true;}
	if (i < 0){	i = 0; Warn = true;}
	if (j >= Height){	j = Height - 1; Warn = true;}
	if (j < 0){	j = 0; Warn = true;}
	if (Warn && EasyBMPwarnings)
	{
		cout << "EasyBMP Warning: Attempted to access non-existent pixel;" << endl
			<< "                 Truncating request to fit in the range [0,"
			<< Width - 1 << "] x [0," << Height - 1 << "]." << endl;
	}
	return &(Pixels[i][j]);
}
bool BMP::Read1bitRow(ebmpBYTE* Buffer, int BufferSize, int Row)
{
	int Shifts[8] = { 7, 6, 5, 4, 3, 2, 1, 0 };
	int Masks[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

	int i = 0;
	int j;
	int k = 0;

	if (Width > 8 * BufferSize)
	{
		return false;
	}
	while (i < Width)
	{
		j = 0;
		while (j < 8 && i < Width)
		{
			int Index = (int)((Buffer[k] & Masks[j]) >> Shifts[j]);
											
			
			*(this->operator()(i, Row)) = GetColor(Index);
			i++; j++;
		}
		k++;
	}
	return true;
}
*/
/////////////////////////////////////////////////////////////////

static bool bmp_get_row_data_for_4(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	unsigned char pos_weights[2] = { 16, 1 };
	unsigned int i = 0, j, k = 0;
	unsigned int index;

	if (bmp->dib.width > 2 * buffer_length) {
		return false;
	}

	while (i < bmp->dib.width){
		for (j = 0, index = 0; j < 2 && i < bmp->dib.width; ++i, ++j){
			index += pos_weights[j] * find_closest_color(bmp, bmp->pixels[i][row]);
		}
		buffer[k++] = index & 0xFF;
	}
	return true;
}

static bool bmp_get_row_data_for_8(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;
	if (bmp->dib.width > buffer_length){
		return false;
	}

	for (i = 0; i < bmp->dib.width; ++i){
		buffer[i] = find_closest_color(bmp, bmp->pixels[i][row]);
	}
	return true;
}

static bool bmp_get_row_data_for_24(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;

	if (bmp->dib.width * 3 > buffer_length){
		return false;
	}
	for (i = 0; i < bmp->dib.width; ++i){
		memcpy(buffer + 3 * i, (unsigned char *)&(bmp->pixels[i][row]), 3);
	}
	return true;
}

static bool bmp_get_row_data_for_32(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;

	if (bmp->dib.width * 4 > buffer_length){
		return false;
	}

	for (i = 0; i < bmp->dib.width; ++i){
		memcpy(buffer + 4 * i, (unsigned char *)&(bmp->pixels[i][row]), 4);
	}
	return true;
}

#define INT_SQUARE(v) ((int)((v) * (v)))

static int find_closest_color(bmpFile *bmp, rgb_pixel pixel){
	int i, best = 0;
	int best_match = 999999;

	for (i = 0; i < bmp->dib.nr_colors; ++i){
		rgb_pixel color = bmp->colors[i];
		int temp_match = INT_SQUARE(color.red - pixel.red) +
			INT_SQUARE(color.green - pixel.green) +
			INT_SQUARE(color.blue - pixel.blue);
		if (temp_match < best_match){
			best = i;
			best_match = temp_match;
		}

		if (best_match < 1){
			break;
		}
	}
	return best;
}

/*
 Clear the memory
*/
void bmp_destroy(bmpFile *bmp){
	// free le pixels
	for (int i = 0; i < bmp->dib.width; ++i){
		free(bmp->pixels[i]);
	}
	free(bmp->pixels), bmp->pixels = NULL;
	// Free le colors
	if (bmp->colors){
		free(bmp->colors);
	}
	free(bmp);

}