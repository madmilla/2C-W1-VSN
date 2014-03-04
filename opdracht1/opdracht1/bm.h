#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
* bm.c
* So some reason i though this was a good idea.
* DIB = Device independant Bitmap	http://msdn.microsoft.com/en-us/library/windows/desktop/dd183562(v=vs.85).aspx
* BMP = Bitmap						http://msdn.microsoft.com/en-us/library/windows/desktop/dd183371(v=vs.85).aspx
*
* libbmp - BMP library
*
*/


/*
* BMP File Header  Stores general information about the BMP file.
* DIB header       Stores detailed information about the bitmap image.
* Color Palette    Stores the definition of the colors being used for
*                    indexed color bitmaps.
* Bitmap Data      Stores the actual image, pixel by pixel.
*/
typedef enum {
	BI_RGB = 0,
	BI_RLE8,
	BI_RLE4,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
} bmp_compression_method_t;


typedef struct {
	unsigned char signature[2];   /* the magic number used to identify the BMP file:
								  0x42 0x4D (Hex code points for B and M).
								  The following entries are possible:
								  BM - Windows 3.1x, 95, NT, ... etc
								  BA - OS/2 Bitmap Array
								  CI - OS/2 Color Icon
								  CP - OS/2 Color Pointer
								  IC - OS/2 Icon
								  PT - OS/2 Pointer. */
	unsigned int filesize;    /* the size of the BMP file in bytes */
	unsigned short reserved1;  /* reserved. */
	unsigned short reserved2;  /* reserved. */
	unsigned int offset;    /* the offset, i.e. starting address,
							of the byte where the bitmap data can be found. */
} bmp_header;

typedef struct {
	unsigned int header_size;     /* the size of this header (40 bytes) */
	unsigned int width;         /* the bitmap width in pixels */
	unsigned int height;        /* the bitmap height in pixels */
	unsigned short nr_planes;       /* the number of color planes being used. Must be set to 1. */
	unsigned short bits_per_pixel;         /* the number of bits per pixel,
										   which is the color depth of the image.
										   Typical values are 1, 4, 8, 16, 24 and 32. */
	unsigned int compress_type; /* the compression method being used.
								See also bmp_compression_method_t. */
	unsigned int bmp_bytesize;    /* the image size. This is the size of the raw bitmap
								  data (see below), and should not be confused
								  with the file size. */
	unsigned int hres;          /* the horizontal resolution of the image.
								(pixel per meter) */
	unsigned int vres;          /* the vertical resolution of the image.
								(pixel per meter) */
	unsigned int nr_colors;       /* the number of colors in the color palette,
								  or 0 to default to 2<sup><i>n</i></sup>. */
	unsigned int nr_imp_colors;    /* the number of important colors used,
								   or 0 when every color is important;
								   generally ignored. */
} bmp_dib_header;
typedef struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
} rgb_pixel;

struct bmpFile{
	bmp_header header;
	bmp_dib_header dib;
	rgb_pixel **pixels;
	rgb_pixel *colors;

};
/*
Create a grayscale version of the bmp
*/

static void bmp_create_grayscale(bmpFile *bmp);

/*
Getters
*/
bmpFile * bmp_open(const char *filename);
unsigned int bmp_get_width(bmpFile *bmp);

unsigned int bmp_get_height(bmpFile *bmp);

unsigned int bmp_get_depth(bmpFile *bmp);

rgb_pixel *bmp_get_pixel(bmpFile *bmp, unsigned int x, unsigned int y);
bool bmp_set_pixel(bmpFile *bmp, unsigned int x, unsigned int y, rgb_pixel pixel);

bool bmp_save(bmpFile *bmp, const char *filename);

static void bmp_write_header(bmpFile *bmp, FILE *fp);

static void bmp_write_dib(bmpFile *bmp, FILE *fp);

static void bmp_write_palette(bmpFile *bmp, FILE *fp);

bmpFile * bmp_create(unsigned int width, unsigned int height, unsigned int bits_per_pixel);

static void bmp_malloc_pixels(bmpFile *bmp);

static void bmp_malloc_colors(bmpFile *bmp);

static void bmp_create_standard_color_table(bmpFile *bmp);

static void bmp_create_grayscale_color_table(bmpFile *bmp);


static unsigned int unsignedint_pow(unsigned int base, unsigned int bits_per_pixel);

/*
Get row data
*/
static void bmp_get_row_data_for_1(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row);

static void bmp_get_row_data_for_4(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row);

static void bmp_get_row_data_for_8(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row);

static void bmp_get_row_data_for_24(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row);

static void bmp_get_row_data_for_32(bmpFile *bmp, unsigned char *buffer, unsigned int buffer_length, unsigned int row);

#define INT_SQUARE(v) ((int)((v) * (v)))

static int find_closest_color(bmpFile *bmp, rgb_pixel pixel);

/*
Clear the memory
*/
void bmp_destroy(bmpFile *bmp);
