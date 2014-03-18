#include <iostream>
#include "FreeImage.h"

using namespace std;

int main(int argc, char* argv[]){

	/*
		1. Een beeld(argument 1) inleest, naar grijswaarden converteert, en opslaat onder de naam “grey_” + [original filename].
		2. Twee genormaliseerde histogrammen van dit grijze beeld berekent en opslaat als csv(comma separated values) file, met op elke regel : “[binnr], [density]”.
		a.De eerste met 256 bins(van 0 tot 255).
		b.De tweede met 10 bins(van 0 tot 9), waarbij het binnummer bij een intensiteit berekend kan worden door : “(int) ((intensiteit * 10) / 256)”.
		3. Histogram equalization van het grijze beeld uitvoert(op basis van de 256 bins) en de uitkomst opslaat in een bestand met de naam “equalized_[original filename]”.
		Experimenteer op een aantal voorbeelden en beschrijf in deze in je meetrapport.
		4. Output bij het eerste programma ook de beelden “R_”+[original filename], “G_”+[original filename] en “B_”+[original filename], waarop resp. alleen het rode, groene en blauwe kanaal te zien is.
		5. Bereken de 10-bin histogrammen voor het rode, groene en blauwe kanaal van het inputbeeld en output deze als csv-file, (het binnummer kan berekend worden door: “(int) ((intensiteit_kanaal * 10) / 256)”).
	*/

	for (int i = 1; i < argc; i++)
		cout << "argv[" << i << "] = " << argv[i] << endl;

	if (argv[1] == NULL){
		FreeImage_Initialise();
		cout << "FreeImage " << FreeImage_GetVersion() << "nn";
		cout << FreeImage_GetCopyrightMessage() << "nnnn";
		FreeImage_DeInitialise();

		return 666;
	}
	else{
		return 0;
	}
}