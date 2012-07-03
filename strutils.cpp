#include "strutils.h"
#include "string.h"

bool stringContains(char* string, char c) {
	int len = strlen(string);

	for (int i = 0; i < len; i++) {
		if (string[i] == c) {
			return true;
		}
	}

	return false;	
}

bool isDelimiter(char c, char* delimiters) {
	int nDelimiters = strlen(delimiters);

	for (int i = 0; i < nDelimiters; i++) {
		if (delimiters[i] == c)
			return true;
	}

	return false;
}

char** stringSplit(char* string, char* delimiters, int* nTokens) {
	char* p = string;
	char* token;
	char** tokens;
	int count = 0;
	bool delimiterFound = false;
	
	while(*p != 0) {
		if (isDelimiter(*p, delimiters) && !delimiterFound) {
			count++;
			delimiterFound = true;
		}
		else {
			delimiterFound = false;
		}
		
		p++;
	}
	
	count++;
	
	tokens = (char**)malloc(count*sizeof(char*));
	*nTokens = count;
	delimiterFound = false;
	token = p = string;
	count = 0;
	
	while(*p != 0) {
		if (isDelimiter(*p, delimiters) && !delimiterFound) {
			*p = 0;
			tokens[count++] = token;
			token = p + 1;
			delimiterFound = true;
		}
		else if (isDelimiter(*p, delimiters) && delimiterFound) {
			token++;
		}
		else {
			delimiterFound = false;
		}
		
		p++;
	}

	tokens[count] = token;
	
	return tokens;
}