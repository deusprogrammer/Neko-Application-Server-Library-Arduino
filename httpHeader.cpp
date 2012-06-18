#include "httpHeader.h"
#include "strutils.h"

HTTPHeader::HTTPHeader() {
}

void HTTPHeader::consumeLine(char* line) {
	int nTokens;
	char** tokens = stringSplit(line, " :", &nTokens);
	
	if (strcmp(tokens[0], "GET")==0 || strcmp(tokens[0], "PUT")==0 || strcmp(tokens[0], "POST")==0 || strcmp(tokens[0], "DELETE")==0) {
		if (strcmp(tokens[0], "GET")==0)
			verb = GET;
		else if (strcmp(tokens[0], "PUT")==0)
			verb = PUT;
		else if (strcmp(tokens[0], "POST")==0)
			verb = POST;
		else if(strcmp(tokens[0], "DELETE")==0)
			verb = DELETE;

		if (stringContains(tokens[1], '?')) {
			int nQTokens;
			char** qTokens = stringSplit(tokens[1], "?&", &nQTokens);
			
			for (int i = 0; i < nQTokens; i++) {
				int nElements;
				char** qElements = stringSplit(qTokens[i], "=", &nElements);

				if (nElements == 2 && nPairs < MAX_PAIRS) {
					strcpy(this->pairs[nPairs].key, qElements[0]);
					strcpy(this->pairs[nPairs++].value, qElements[1]);
				}

				free(qElements);
			}

			free(qTokens);
		}

		strcpy(this->resource, tokens[1]);
	}
	else if (strcmp(tokens[0], "Content-Length") == 0) {
		contentLength = atoi(tokens[1]);
	}

	free(tokens);
}

char* HTTPHeader::fetchQuery (char* key) {
	for (int i=0; i < this->nPairs; i++) {
		if (strcmp(this->pairs[i].key, key) == 0) {
			return this->pairs[i].value;
		}
	}
}