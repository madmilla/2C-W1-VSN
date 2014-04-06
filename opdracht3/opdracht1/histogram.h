#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "FreeImage.h"
#include <string>
#include "util.h"



/**
* histogram.c
*
*
* Author: Lars Veenendaal
* TIV-2A 1633223
*
* Making histograms since march 2014
*
*/

void saveHistogramToCsv(int limit, int array[], char *prefix, char *filename);