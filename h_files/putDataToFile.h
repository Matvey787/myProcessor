#ifndef PUTDATATOFILE
#define PUTDATATOFILE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void putDataToFile(double* array, size_t length, const char* file_name);

#endif