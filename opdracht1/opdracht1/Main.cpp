#include <iostream>
#include <fstream>
#include "FreeImage.h"
#include <string.h>
#include "bm.h"

using namespace std;

bool check_for_lack_of_first_parameter(char* first_parameter){
	return (first_parameter == NULL)? true : false;
}
bool check_for_extra_parameters(char * second_paramter){
	return (second_paramter == NULL) ? false : true;
}
bool check_file_existance(char * name){
	ifstream myfile(name);
	if (myfile.is_open())
	{
		myfile.close();
		return true;
	}
	else{
		return false;
	}
}

int main(int argc, char* argv[]){

	/*
	@author Lars Veenendaal lars.veenendaal@student.hu.nl 1633223

	Schrijf een command-line-programma dat achtereenvolgens
	1. Een beeld (argument 1) inleest, naar grijswaarden converteert, en opslaat onder de naam “grey_”+[original filename].
	2. Twee genormaliseerde histogrammen van dit grijze beeld berekent en opslaat als csv (comma separated values) file, met op elke regel: “[binnr], [density]”.
	a. De eerste met 256 bins (van 0 tot 255).
	b. De tweede met 10 bins (van 0 tot 9), waarbij het binnummer bij een intensiteit berekend kan worden door: “(int) ((intensiteit * 10) / 256)”.
	3. Histogram equalization van het grijze beeld uitvoert (op basis van de 256 bins) en de uitkomst opslaat in een bestand met de naam “equalized_[original filename]”.
	filename]”.
	Experimenteer op een aantal voorbeelden en beschrijf in deze in je meetrapport.
	4. Output bij het eerste programma ook de beelden “R_”+[original filename], “G_”+[original filename] en “B_”+[original filename], waarop resp. alleen het rode, groene en blauwe kanaal te zien is.
	5. Bereken de 10-bin histogrammen voor het rode, groene en blauwe kanaal van het inputbeeld en output deze als csv-file, (het binnummer kan berekend worden door: “(int) ((intensiteit_kanaal * 10) / 256)”).
	*/

	if (check_for_lack_of_first_parameter(argv[1]))
	{
		printf("This program requires a single parameter:\n");
		printf("opdract1 iamtheonewhoknocks.jpg");
		
		bmpFile *bmp;
		bmp = bmp_open("test4.bmp");
		int i, j;
		rgb_pixel pixel = { 128, 64, 0, 0 };

		//if (argc < 5) {
		//	printf("Usage: %s filename width height depth.\n", argv[0]);
		//	return 1;
		//}

	//	if ((bmp = bmp_create(1024, 1024, 24)) == NULL) {
	//		printf("Invalid depth value: %s.\n", 24);
//			return 1;
	//	}

		for (i = 10, j = 10; j < 1024; ++i, ++j) {
			bmp_set_pixel(bmp, i, j, pixel);
			pixel.red++;
			pixel.green++;
			pixel.blue++;
			bmp_set_pixel(bmp, i + 1, j, pixel);
			bmp_set_pixel(bmp, i, j + 1, pixel);
		}

		bmp_save(bmp, "artorias.bmp");
		bmp_destroy(bmp);
		return 0;

	}else if (check_for_extra_parameters(argv[2]))
	{
		printf("This program requires a single parameter\n");
		printf("any after the first parameter will be ignored.");
		return 2;
	}
	if (check_file_existance(argv[1])){
		char * filename = argv[1];
		printf("File succesfully opened.\n");
		FreeImage_Initialise();
		cout << "FreeImage " << FreeImage_GetVersion() << "\n";
		cout << FreeImage_GetCopyrightMessage() << "\n";

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename, 0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, filename);

		FIBITMAP* temp = FreeImage_ConvertToGreyscale(imagen);
		// myeah werkt niet : (sizeof(filename)+6)
		char prefix[256] = "GREY_";
			FreeImage_Save(FreeImage_GetFileType(filename), temp, strcat(prefix, filename));
	
		int w = FreeImage_GetWidth(imagen);
		int h = FreeImage_GetHeight(imagen);
		cout << "The size of the image is: " << argv[1] << " es " << w << "*" << h << endl; //Some debugging code
		FreeImage_Unload(imagen);
		FreeImage_DeInitialise();
		return 0;
	}
	else {
		printf("File couldn't be opened.\n");
		return 3;
	}

	return 0;
}