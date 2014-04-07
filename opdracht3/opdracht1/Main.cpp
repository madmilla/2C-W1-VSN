
#include <iostream>
#include <fstream>
#include <string.h>
#include "FreeImage.h"
#include "image.h"
#include "kmeans.h"
#include "thresholding.h"
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
	cout << "K-MEANS algoritm" << endl;
	cout << "2nd parameter is the amount of clusters" << endl;
	cout << "3rd parameter is the amount of iterations" << endl;
	cout << "opdracht3.exe alex.jpg 7 20\n" << endl;
	cout << "To access iterative thresholding make sure the 2nd parameter = 255" << endl;
}

int main(int argc, char* argv[]){
	/*
	@author Lars Veenendaal lars.veenendaal@student.hu.nl 1633223

	Schrijf een command-line-programma dat achtereenvolgens:
	a. Een kleurenbeeld inleest (bestandsnaam: 1e parameter) en in RGB-representatie intern opslaat.
	b. De RGB-waarden van alle pixels clustert in het aangegeven aantal (k: 2e parameter) clusters, met als stopconditie:
		i. de clustermiddelpunten veranderen niet meer, of
		ii. het maximum van 25 iteraties wordt overgeschreden.
	c. Elk pixel de kleur geeft van het clustermiddelpunt van het cluster waartoe dat pixel behoort. (Zie voor uitleg over het k-means clustering algoritme de slides van les 3 .)
	2. Pas dit programma toe op een kleurenfoto naar keuze voor k=3,5,7 en 20. Maak hiervan een meetrapport.
	3. Implementeer het algoritme uit de les voor het automatisch bepalen van een drempelwaarde. 
	Voer thresholding uit op een grijswaardenbeeld op basis van de automatisch uitgerekende drempel. 
	Voer experimenten uit om te kijken wat de resultaten zijn van deze methode en laat de resultaten van de experimenten zien in een meetrapport.*/

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
		int Amount_of_clusters = atoi(argv[2]);
		if (Amount_of_clusters == 255){
			temp.ConvertToGreyscale(3);
			Thresholding Thres_image = temp;
			Thres_image.iterativeThresholding();
			Thres_image.save_image(FreeImage, "THRES_", filename);
		} else {
			int Amount_of_clusters = atoi(argv[2]);
			int iterations = 1;
			iterations = atoi(argv[3]);
			kMeans kMeans_image = temp;
			kMeans_image.clusteringByKMeans(Amount_of_clusters, iterations);
			kMeans_image.save_image(FreeImage, "KMEANS_", filename);
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