#include <stdio.h>
#include <stdlib.h>

#include "../h_files/getNumber.h"

void clearInput();

void getCoeff(const char* message, double* num){
    int c=0;
    int maxAttempts = 2;
	while (printf("%s", message) && ((c = fscanf(stdin, "%lg", num)) != 1) && (c != EOF) && maxAttempts >= 1){
		clearInput();
		--maxAttempts;
		}
    if (maxAttempts == 0)
        {
        printf("Sorry, you have already used all attempts!");
        exit(1);
        }
	clearInput();
}

void clearInput() {
	while (getchar() != '\n');
}