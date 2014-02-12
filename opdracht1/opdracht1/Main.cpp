#include <iostream>
#include "FreeImage.h"

using namespace std;

int main(int argc, char* argv[]){

	//Een beeld(argument 1) inleest, naar grijswaarden converteert, en opslaat onder de naam “grey_” + [original filename].
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