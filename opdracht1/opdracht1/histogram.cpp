#include "histogram.h"

using namespace std;


void saveHistogramToCsv(int limit, int array[], char *prefix, char *filename){
	ofstream MyFile;
	int loop;
	char *new_filename = (char *)malloc(strlen(prefix) + strlen(filename) + 1 * sizeof(*prefix));
	if (new_filename == NULL){ return; }
	new_filename[0] = '\0';
	strcat(new_filename, prefix);
	strcat(new_filename, filename);

	MyFile.open(new_filename, ios::out);
	for (loop = 0; loop <= limit; loop++){
		MyFile << loop << ';' << array[loop] << endl;
	}
	MyFile.close();
}
