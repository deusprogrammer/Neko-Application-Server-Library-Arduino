#ifndef STRUTILS_H
#define STRUTILS_H

#include "stdlib.h"

bool stringContains(char* string, char c);
bool isDelimiter(char c, char* delimiters);
char** stringSplit(char* string, char* delimiters, int* nTokens);
#endif