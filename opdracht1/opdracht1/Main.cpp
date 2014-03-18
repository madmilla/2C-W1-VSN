#include <iostream>
#include <fstream>
#include <string.h>
#include "FreeImage.h"
#include "manipulate.h"
#include "histogram.h"
#include "util.h"
#include "image.h"

using namespace std;

bool check_for_lack_of_first_parameter(char* first_parameter){
	return (first_parameter == NULL) ? true : false;
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
	enum greyScale { general, lightness, luminosity };
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
		printf("Missing parameters");
		return 0;

	}
	else if (check_for_extra_parameters(argv[2]))
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

		int result_array_256[256];
		int result_array_10[11];
		FIBITMAP * FreeImage = FreeImage_Load(formato, filename);
        Image temp = Image(FreeImage);
		Image temp_copy_1 = temp, temp_copy_2 = temp, temp_copy_3 = temp, temp_copy_4 = temp, temp_copy_5 = temp, temp_copy_6 = temp, temp_copy_7 = temp;
		temp_copy_1.getRGBChannelSeperatedIntensity(filename);

		temp_copy_2.getGreenChannelOnly();
		temp_copy_3.getBlueChannelOnly();
		temp_copy_1.save_image(FreeImage, "R_", filename);
		temp_copy_2.save_image(FreeImage, "G_", filename);
		temp_copy_3.save_image(FreeImage, "B_", filename);

		temp_copy_4.ConvertToGreyscaleWithDensityArray(result_array_256, result_array_10, general);
		//temp_copy_4.ConvertToGreyscale();
		temp_copy_4.save_image(FreeImage, "GR_", filename);
		temp_copy_5.ConvertToGreyscale(lightness);
		temp_copy_5.save_image(FreeImage, "GR_LI_", filename);
		temp_copy_6.ConvertToGreyscale(luminosity);
		temp_copy_6.save_image(FreeImage, "GR_LU_", filename);

		
		int loop = 0;
		int eq_array[256];
		
		for (loop = 0; loop < 256; loop++){
			eq_array[loop] = 0;
		}
		int w = temp_copy_7.getWidth();
		int h = temp_copy_7.getHeight();
		
		float scaling_factor = 255.0f / (w*h);
		printf("sf: %f %i %i \n", scaling_factor, w, h);
		eq_array[0] = scaling_factor *  result_array_256[0];
		for (loop = 1; loop < 256; loop++){
			eq_array[loop] = eq_array[loop - 1] + scaling_factor * result_array_256[loop];
		}
		/*
		for (loop = 0; loop < 11; loop++){
			eq_array[loop] = 0;
		}
		int w = temp_copy_7.getWidth();
		int h = temp_copy_7.getHeight();

		float scaling_factor = 255.0f / (w*h);
		printf("sf: %f %i %i \n", scaling_factor, w, h);
		eq_array[0] = scaling_factor *  result_array_10[0];
		for (loop = 1; loop < 11; loop++){
			eq_array[loop] = eq_array[loop - 1] + scaling_factor * result_array_10[loop];
		}
		*/
		temp_copy_7.equalizeHistogram(eq_array, luminosity);
		temp_copy_7.save_image(FreeImage, "GR_EQ_", filename);

		saveHistogramToCsv(255, result_array_256, filename, "_256.csv");
		saveHistogramToCsv(10, result_array_10, filename, "_10.csv");
		saveHistogramToCsv(10, eq_array, filename,"_eq.csv");

//		cout << "The size of the image is: " << argv[1] << " es " << w << "*" << h << endl; //Some debugging code

		FreeImage_Unload(FreeImage);
		FreeImage_DeInitialise();
		return 0;
	}
	else {
		printf("File couldn't be opened.\n");
		return 3;
	}

	return 0;
}