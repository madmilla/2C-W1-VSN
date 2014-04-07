
#include <iostream>
#include <fstream>
#include <string.h>
#include "FreeImage.h"
#include "image.h"

#include "mapping.h"
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
	cout << "Backwardmapping algoritm" << endl;
	cout << "2nd parameter a file with a matrix inside\nmatrix.txt rotating.txt moving.txt" << endl;
	cout << "3th parameter Zero of First order interpolation " << endl;
	cout << "opdracht3.exe alex.jpg moving.txt 1\n" << endl;
}

int main(int argc, char* argv[]){
	enum greyScale { general, lightness, luminosity };
	/*
	@author Lars Veenendaal lars.veenendaal@student.hu.nl 1633223

	1. Schrijf een command-line-programma dat achtereenvolgens:
		DONE a. Een kleurenbeeld inleest (bestandsnaam: 1e parameter) en in RGB-representatie intern opslaat.
		DONE b. Een 3x3 transformatiematrix inleest (bestandsnaam: 2e parameter).
		DPME c. Een interpolatiemethode inleest (3e parameter; 0=zero-order en 1=first-order)
		d. De transformatie op het beeld toepast via backward mapping.
	2. Pas dit programma toe op een kleurenfoto naar keuze, voor de volgende transformaties:
		DONE a. Schalen met factor 3,5.
		DONE b. Rotatie met een hoek van 20 graden (=1/9*π).
		DONE MOVING c. Een willekeurige andere transformatiematrix.

		Alle drie de bovenstaande transformaties voer je tweemaal uit (eenmaal met elk van de twee interpolatiemethoden). 
		Maak hiervan een meetrapport. 
		Denk eraan dat het originele beeld en de outputbeelden opgenomen zijn in de werkwijze van het meetrapport. 
		Bespreek de uitkomst en wat je eraan opvalt.
	3.	? DONE denk ik Implementeer het verruimen van de coördinaatruimte, 
		en het transleren van een beeld na een rotatie, zodanig dat het geroteerde beeld wel geheel te zien blijft 
		(zie pagina 243 van de pdf Geometric operations).*/

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
	}
	else if (check_file_existance(argv[1]) && check_file_existance(argv[2])){
		char * filename = argv[1];
		char * matrix = argv[2];
		printf("File succesfully opened.\n");
		FreeImage_Initialise();
		cout << "FreeImage " << FreeImage_GetVersion() << "\n";
		cout << FreeImage_GetCopyrightMessage() << "\n";
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename, 0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP * FreeImage = FreeImage_Load(formato, filename);
        Image temp = Image(FreeImage);
		Mapping map_image = temp;

		double myMatrix[9];
		double matrix_adjecent[9];
		ifstream file(matrix);
		if (file.is_open())
		{
			for (int i = 0; i < 9; ++i)
			{
				matrix_adjecent[i] = 0;
				file >> myMatrix[i];
			}
		}
		/* Test block. 
		int matrix_determinant = (myMatrix[0] * ((myMatrix[4] * myMatrix[8]) - (myMatrix[5] * myMatrix[7]))) - (myMatrix[1] * ((myMatrix[3] * myMatrix[8]) - (myMatrix[5] * myMatrix[6]))) + (myMatrix[2] * ((myMatrix[3] * myMatrix[7]) - (myMatrix[4] * myMatrix[6])));
		cout << matrix_determinant << endl;
		matrix_adjecent[0] = (myMatrix[4] * myMatrix[8]) - (myMatrix[5] * myMatrix[7]);
		matrix_adjecent[1] = (myMatrix[3] * myMatrix[8]) - (myMatrix[5] * myMatrix[6]); matrix_adjecent[1] = (matrix_adjecent[1] < 0) ? (matrix_adjecent[1] * -1) : matrix_adjecent[1];
		matrix_adjecent[2] = (myMatrix[3] * myMatrix[7]) - (myMatrix[4] * myMatrix[6]);

		matrix_adjecent[3] = (myMatrix[1] * myMatrix[8]) - (myMatrix[2] * myMatrix[7]); matrix_adjecent[3] = (matrix_adjecent[3] < 0) ? (matrix_adjecent[3] * -1) : matrix_adjecent[7];
		matrix_adjecent[4] = (myMatrix[0] * myMatrix[8]) - (myMatrix[2] * myMatrix[6]);
		matrix_adjecent[5] = (myMatrix[0] * myMatrix[7]) - (myMatrix[2] * myMatrix[6]); matrix_adjecent[5] = (matrix_adjecent[5] < 0) ? (matrix_adjecent[5] * -1) : matrix_adjecent[7];

		matrix_adjecent[6] = (myMatrix[1] * myMatrix[5]) - (myMatrix[2] * myMatrix[4]);
		matrix_adjecent[7] = (myMatrix[0] * myMatrix[5]) - (myMatrix[2] * myMatrix[3]); matrix_adjecent[7] = (matrix_adjecent[7] < 0) ? (matrix_adjecent[7] * -1) : matrix_adjecent[7];
		matrix_adjecent[8] = (myMatrix[0] * myMatrix[4]) - (myMatrix[1] * myMatrix[3]);
		*/
		char * temp_var = argv[2];
		int interpolatie = 0;
		if (!check_for_lack_of_first_parameter(argv[3]))
		{
			interpolatie = atoi(argv[3]);
		}
		if (interpolatie == 0){
			cout << "interpolatie 0" << endl;
			map_image.transformMapping(myMatrix, 0);
			map_image.save_image(FreeImage, "MAP_0_", filename);
		}
		else if (interpolatie == 1){
			cout << "interpolatie 1" << endl;
			map_image.transformMapping(myMatrix, 1);
			map_image.save_image(FreeImage, "MAP_1_", filename);
		}

		cout << "Method ended. Exiting program." << endl;
		FreeImage_Unload(FreeImage);
		FreeImage_DeInitialise();
		return 0;
	}
	else {
		printf("File (Image) couldn't be opened.\n");
		return 3;
	}

	return 0;
}