#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include "bm.h"

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
	
	for (i = 0; i < bmp->dib.nr_colors; ++i){
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

bmpFile * bmp_open(const char *filename){
	FILE *fp;
	bmpFile *bmp;


	int row;
	unsigned char *buffer;
	if ((fp = fopen(filename, "r")) == NULL){
		return false;
	}
	unsigned char * datBuff[2] = { nullptr, nullptr }; // Header buffers
	unsigned char* pixels = nullptr;	// Pixels
	bmpFile bmp; // Header
	bmp_dib_header* bmpInfo = nullptr;	// dib 


	datBuff[0] = new unsigned char[sizeof(bmp_header)];
	datBuff[1] = new unsigned char[sizeof(bmp_dib_header)];
	std::ifstream file(filename, std::ios::binary);
	file.read((char*)datBuff[0], sizeof(bmp_header));
	file.read((char*)datBuff[1], sizeof(bmp_dib_header));

	//Construct the values from the buffers
	bmp->header->signature[0] = (bmp_header*)datBuff[0]->signature[1];
	bmpInfo = (bmp_dib_header*)datBuff[1];
	//0x42 is 'B' and 0x4D is 'M'
	if (bmp->header->signature[0] != 'B' || bmp->header->signature[1] != 'M'){
		// This is not a bitmap.
		return NULL;
	}
	// First allocate pixel memory
	pixels = new unsigned char[bmpInfo->bmp_bytesize];

	// Go to where image data starts then read in image data
	file.seekg(bmpHeader->offset);
	file.read((char*)pixels, bmpInfo->bmp_bytesize);

	// Now that we can loadand read it guess what the flipping format is off.
	// Someone decided storing as BGR was a good idea. 
	// Lets flip the fucker.

	unsigned char tempRGB = 0;
	unsigned long i;
	for (i = 0; i < bmpInfo->bmp_bytesize; i += 3){
		tempRGB = pixels[i];
		pixels[i] = pixels[i];
		pixels[i + 2] = tempRGB;
	}

	//bmp = (bmpFile *)malloc(sizeof(bmpFile));
	
	return bmp;
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

	fwrite(header.signature, sizeof(header.signature), 1, fp);
	fwrite(&(header.filesize), sizeof(unsigned int), 1, fp);
	fwrite(&(header.reserved1), sizeof(unsigned short), 1, fp);
	fwrite(&(header.reserved2), sizeof(unsigned short), 1, fp);
	fwrite(&(header.offset), sizeof(unsigned int), 1, fp);
}

static void bmp_write_dib(bmpFile *bmp, FILE *fp){
	bmp_dib_header dib = bmp->dib;

	fwrite(&(dib.header_size), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.width), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.height), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_planes), sizeof(unsigned short), 1, fp);
	fwrite(&(dib.bits_per_pixel), sizeof(unsigned short), 1, fp);
	fwrite(&(dib.compress_type), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.bmp_bytesize), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.hres), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.vres), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_colors), sizeof(unsigned int), 1, fp);
	fwrite(&(dib.nr_imp_colors), sizeof(unsigned int), 1, fp);
}

static void bmp_write_palette(bmpFile *bmp, FILE *fp){
	if (bmp->dib.bits_per_pixel == 1 || bmp->dib.bits_per_pixel == 4 || bmp->dib.bits_per_pixel == 8){
		for (int i = 0; i < bmp->dib.nr_colors; ++i){
			fwrite(&(bmp->colors[i]), sizeof(rgb_pixel), 1, fp);
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
	result->dib.hres = DEFAULT_DPI_X;
	result->dib.vres = DEFAULT_DPI_Y;

	if (bits_per_pixel == 16){
		result->dib.compress_type = BI_BITFIELDS;
	}
	else {
		result->dib.compress_type = BI_RGB;
	}

	bmp_malloc_pixels(result);
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

static void bmp_malloc_pixels(bmpFile *bmp){
	int i, j;

	bmp->pixels = (rgb_pixel **)malloc(sizeof(rgb_pixel *)* bmp->dib.width);
	for (i = 0; i < bmp->dib.width; ++i){
		bmp->pixels[i] = (rgb_pixel *)malloc(sizeof(rgb_pixel)* bmp->dib.height);
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
	if (bmp->dib.bits_per_pixel == 1 || bmp->dib.bits_per_pixel == 4 || bmp->dib.bits_per_pixel == 6){
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
static void bmp_get_row_data_for_1(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	unsigned char pos_weight[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	unsigned int i = 0, j, k = 0;
	unsigned int index;

	if (bmp->dib.width > 8 * buffer_length){
		return;
	}

	while (i < bmp->dib.width){
		for (j = 0, index = 0; j < 8 && i < bmp->dib.width; ++i, ++j){
			index += pos_weight[j] * find_closest_color(bmp, bmp->pixels[i][row]);
		}
		buffer[k++] = index & 0xff;
	}

}

static void bmp_get_row_data_for_4(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	unsigned char pos_weights[2] = { 16, 1 };
	unsigned int i = 0, j, k = 0;
	unsigned int index;

	if (bmp->dib.width > 2 * buffer_length) {
		return;
	}

	while (i < bmp->dib.width){
		for (j = 0, index = 0; j < 2 && i < bmp->dib.width; ++i, ++j){
			index += pos_weights[j] * find_closest_color(bmp, bmp->pixels[i][row]);
		}
		buffer[k++] = index & 0xFF;
	}
}

static void bmp_get_row_data_for_8(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;
	if (bmp->dib.width > buffer_length){
		return;
	}

	for (i = 0; i < bmp->dib.width; ++i){
		buffer[i] = find_closest_color(bmp, bmp->pixels[i][row]);
	}
}

static void bmp_get_row_data_for_24(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;

	if (bmp->dib.width * 3 > buffer_length){
		return;
	}
	for (i = 0; i < bmp->dib.width; ++i){
		memcpy(buffer + 3 * i, (unsigned char *)&(bmp->pixels[i][row]), 3);
	}
}

static void bmp_get_row_data_for_32(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row){
	int i;

	if (bmp->dib.width * 4 > buffer_length){
		return;
	}

	for (i = 0; i < bmp->dib.width; ++i){
		memcpy(buffer + 4 * i, (unsigned char *)&(bmp->pixels[i][row]), 4);
	}
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