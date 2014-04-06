
#include <iostream>
#include <fstream>
#include <string.h>
#include "FreeImage.h"
#include "manipulate.h"
#include "histogram.h"
#include "util.h"
#include "image.h"
#include "noise.h"
#include "median.h"
#include "linkedlist.h"

using namespace std;

bool check_for_lack_of_first_parameter(char* first_parameter){
	return (first_parameter == NULL) ? true : false;
}
bool check_for_extra_parameters(char * second_paramter){
	return (second_paramter == NULL) ? true : false;
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
void showInstructions(){
	cout << "opdracht2.exe alex.jpg 3 10 3\n" << endl;
	cout << "means noise with median and show the succesrate\n" << endl;
	cout << "10% noise and 3x3 mask.\n\n" << endl;
	cout << "1st parameter = filename (alex.jpg)\n" << endl;
	cout << "2nd parameter = method\n" << endl;
	cout << "You can use ether the number or the methodname\n" << endl;
	cout << "1 = NOISE_ONLY\n" << endl;
	cout << "2 = NOISE_MEDIAN\n" << endl;
	cout << "3 = NOISE_MEDIAN_SUCCESRATE\n" << endl;
	cout << "4 = NOISE_MEDIAN_MIN_MAX\n" << endl;
	cout << "3rd parameter = Noise percentage to be added.\n" << endl;
	cout << "4th parameter is the size of median mask (3,5,6,7)." << endl;
}

int main(int argc, char* argv[]){
	enum greyScale { general, lightness, luminosity };
	/*
	@author Lars Veenendaal lars.veenendaal@student.hu.nl 1633223

	Schrijf een command-line-programma dat achtereenvolgens
	6. Schrijf een command-line programma dat achtereenvolgens
		DONE a. Een beeld (argument 1) inleest, er salt&pepper-ruis aan toevoegt, en weer opslaat onder de naam “noise_[original filename]”.
		DONE b. Het median filter toepast op het beeld met ruis, en het resultaat opslaat onder de naam “median_[original filename]”.
	Voer je programma uit op een aantal voorbeelden en beschrijf in je verslag wat je opvalt aan het resultaat.
	NB: Let hierbij goed op de efficiëntie van jouw implementatie van het median filter, omdat je deze ook nodig zult gaan hebben in het project.
	7. Pas het programma aan zodat deze ook
		a. Het minimum-filter toepast op het beeld met ruis, en het resultaat opslaat onder de naam “min_[original filename]”.
		b. Het maximum-filter toepast op het beeld met ruis, en het resultaat opslaat onder de naam “max_[original filename]”.
	Experimenteer met de drie resultaatbeelden en vergelijk deze in je meetrapport.
	*/

	if (check_for_lack_of_first_parameter(argv[1]))
	{
		printf("Missing parameters\n");
		showInstructions();
		return 0;

	}
	else if (check_for_extra_parameters(argv[2]))
	{
		printf("This program requires a multiple parameters\n");
		showInstructions();
		return 2;
	}else if (check_file_existance(argv[1])){
		char * filename = argv[1];
		printf("File succesfully opened.\n");
		FreeImage_Initialise();
		cout << "FreeImage " << FreeImage_GetVersion() << "\n";
		cout << FreeImage_GetCopyrightMessage() << "\n";
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename, 0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP * FreeImage = FreeImage_Load(formato, filename);
        Image temp = Image(FreeImage);
		char * temp_var = argv[2];
		if (atoi(argv[2]) == 1 || temp_var == string("NOISE_ONLY")){
			cout << "Starting NOISE_ONLY" << endl;
			int noise_setting = (argv[3] == NULL) ? 2 : atoi(argv[3]);
			
			Noise noise_image = temp;
			noise_image.addNoise(noise_setting);
			noise_image.save_image(FreeImage, "NOISE_", filename);
		}
		if (atoi(argv[2]) == 2 || temp_var == string("NOISE_MEDIAN")){
			cout << "Starting NOISE_MEDIAN" << endl;
			int noise_setting = (argv[3] == NULL) ? 2 : atoi(argv[3]);
			int median_mask = (argv[4] == NULL) ? 3 : atoi(argv[4]);
			Noise noise_image = temp;
			noise_image.addNoise(noise_setting);
			noise_image.save_image(FreeImage, "NOISE_", filename);
			Median median_image = noise_image;
			median_image.MedianFilterRGB(median_mask);
//			median_image.save_image(FreeImage, "MEDIAN_", filename);
		}
		if (atoi(argv[2]) == 3 || temp_var == string("NOISE_MEDIAN_SUCCESRATE")){
			cout << "Starting NOISE_MEDIAN_SUCCESRATE" << endl;
			LinkedList<int> list;
			int noise_setting = (argv[3] == NULL) ? 2 : atoi(argv[3]);
			int median_mask = (argv[4] == NULL) ? 3 : atoi(argv[4]);
			Noise noise_image = temp;
			noise_image.addNoiseWithNoiseList(noise_setting, list);
			noise_image.save_image(FreeImage, "NOISE_", filename);

			Median median_image = noise_image;
			median_image.MedianFilterRGB(median_mask);
//			median_image.save_image(FreeImage, "MEDIAN_", filename);
			median_image.checkSuccesAgainstNoiseList(list);
		}
		if (atoi(argv[2]) == 4 || temp_var == string("NOISE_MEDIAN_MIN_MAX")){
			cout << "Starting NOISE_MEDIAN_MIN_MAX" << endl;
			cout << "Go get some coffee this will take awhile." << endl;
			cout << "MIN mask is 3x3 and MAX mask is currently 11x11.\n25x25 seemed like overkill to me." << endl;
			cout << "My implementation of median seems like the bogosort method of going at it." << endl;
			int noise_setting = (argv[3] == NULL) ? 2 : atoi(argv[3]);
			Noise noise_image = temp;
			noise_image.addNoise(noise_setting);
			noise_image.save_image(FreeImage, "NOISE_", filename);
			Median median_image_min(noise_image);
			Median median_image_max(noise_image);
			median_image_min.MedianFilterRGB(3);
			median_image_min.save_image(FreeImage, "MIN_", filename);
			median_image_max.MedianFilterRGB(5);
			median_image_max.save_image(FreeImage, "MAX_", filename);
		}
		if (atoi(argv[2]) == 5){
			LinkedList<int> list;
			list.add(1,1,5);
			list.add(1, 1, 5); list.add(1, 1, 5); list.add(1, 1, 5);
			list.print();
		}
		cout << "Method ended. Exiting program." << endl;
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