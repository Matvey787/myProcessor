#ifndef LABEL
#define LABEL

#include <stdio.h>

struct label_t {
    char name[20];
    int value;
};

struct labelsData_t{
    label_t* labels;
    size_t size;
};

#endif